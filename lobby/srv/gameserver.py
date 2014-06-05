"""
* ASCIIWar
* Copyright (c) 2014 Frederic Brachfogel
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Frederic Brachfogel may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY FREDERIC BRACHFOGEL ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL FREDERIC BRACHFOGEL BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
"""

import enet
import struct
from threading import Thread
import sys
import logging
print "asciiwar gameserver starting..."

# --
logging.basicConfig(level=logging.DEBUG,
                    format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
                    datefmt='%m-%d %H:%M',
                    filename='./log',
                    filemode='w+')
console = logging.StreamHandler()
console.setLevel(logging.INFO)
formatter = logging.Formatter('%(name)-12s: %(levelname)-8s %(message)s')
console.setFormatter(formatter)
logging.getLogger('').addHandler(console)

# --
MASTER_SERVER_PORT          = 30000
MASTER_SERVER_PORT_COUNT    = 10000

# --
AW_cmd_type_none            = 0
AW_cmd_type_unit_order      = 1
AW_cmd_type_spawn_unit      = 2
AW_cmd_type_build_unit      = 3
AW_cmd_type_generic         = 4
AW_cmd_type_end_of_turn     = 5
AW_cmd_type_join_game       = 6
AW_cmd_type_seed_game       = 7
AW_cmd_type_leech_game      = 8
AW_cmd_type_ping            = 9

# --
free_ports    = range(MASTER_SERVER_PORT+1, MASTER_SERVER_PORT+MASTER_SERVER_PORT_COUNT)
master_host   = enet.Host(enet.Address(None, MASTER_SERVER_PORT), 32, 1, 0, 0)
games         = dict()
run           = True
logging.info("Running, waiting for connection...")

# --
def game_ready(game_name):
  player_count = games[game_name]['player_count']
  team_count   = games[game_name]['team_count']
  if len(games[game_name]['game_instances']) == team_count * (player_count-1):
    logger = logging.getLogger(game_name)
    logger.info("Game Ready.")
    for i, pl in enumerate(games[game_name]['game_instances']):
      pl.send(0, enet.Packet(struct.pack("<I I I I 8s", i*player_count, i, player_count, team_count, '_READY_\x00'), enet.PACKET_FLAG_RELIABLE))

# --
def players_connected(game_name):
  player_count  = games[game_name]['player_count']
  team_count    = games[game_name]['team_count']
  total_players = player_count * team_count
  if((len(games[game_name]['seeders']) == total_players and 
     len(games[game_name]['leechers']) == total_players) or 
     (len(games[game_name]['seeders']) == total_players and
     games[game_name]['team_count'] == 1)):
    logger = logging.getLogger(game_name)
    logger.info("Players connected.")
    for pl in games[game_name]['game_instances']:
      pl.send(0, enet.Packet('_PLAYERS_CONNECTED_\x00', enet.PACKET_FLAG_RELIABLE))
    games[game_name]['started'] = True

# -- input thread
"""def input_processing():
  global run
  i = sys.stdin.read(1)
  run = False
input_thread = Thread(target = input_processing)
input_thread.start()"""

# -- 
while run: 
  # -- started games
  for game in games.values():
    if game['started']:
      seeders = game['seeders']
      for host, leechers, seed_player_id, _ in seeders:
        done = False
        while not done:
          e = host.service(0)
          if e.type == enet.EVENT_TYPE_RECEIVE:
            # -- todo: optimize (2 ifs)
            if e.packet.dataLength > 0:
              for leech_peer, leech_player_id in leechers:
                if leech_player_id == seed_player_id:
                  leech_peer.send(0, e.packet)
          else:
            logger = logging.getLogger(game_name)
            if e.type == enet.EVENT_TYPE_CONNECT:
              logger.info("%s: SEEDING CONNECT" % e.peer.address)
            elif e.type == enet.EVENT_TYPE_DISCONNECT:
              logger.info("%s: SEEDING DISCONNECT" % e.peer.address)
            else:
              done = True
        host.flush()
  # -- master host 
  master_host.flush
  e = master_host.service(0)
  if e.type == enet.EVENT_TYPE_CONNECT:
    logging.info("%s: CONNECT" % e.peer.address)
  elif e.type == enet.EVENT_TYPE_DISCONNECT:
    logging.info("%s: DISCONNECT" % e.peer.address)
    for game in [g for g in games.values() if str(e.peer.address) in [str(p.address) for p in g['game_instances']]]:
      i = [str(p.address) for p in game['game_instances']].index(str(e.peer.address))
      del game['game_instances'][i]
      if not game['game_instances']:
        logging.info("%s game deleted" % game['game_name'])
        game_name = game['game_name'] 
        del games[game_name]
  elif e.type == enet.EVENT_TYPE_RECEIVE:
    if e.packet.dataLength > 0:
      t, = struct.unpack_from("<I", e.packet.data)

      # -- ping
      if t == AW_cmd_type_ping:
        e.peer.send(0, e.packet) 

      # -- join game
      elif t == AW_cmd_type_join_game:
        t, player_count, team_count, game_name = struct.unpack_from("<I I I 64s", e.packet.data)
        game_name = game_name[:game_name.index('\x00')+1]
        if not game_name in games:
          games[game_name] = {'game_name':          game_name, 
                              'player_count':       player_count, 
                              'team_count':         team_count, 
                              'creator':            e.peer,
                              'game_instances':     [e.peer],
                              'leechers':           [],
                              'seeders':            [],
                              'started':            False}
        else:
          games[game_name]['game_instances'].append(e.peer)
        game_ready(game_name)

      # -- seed game
      elif t == AW_cmd_type_seed_game:
        t, game_name, player_id = struct.unpack_from("<I 64s I", e.packet.data) 
        game_name = game_name[:game_name.index('\x00')+1]
        port = free_ports.pop()
        seeder = (
           enet.Host(enet.Address(None, port), 32, 1, 0, 0), 
           games[game_name]['leechers'], 
           player_id,
           game_name)
        games[game_name]['seeders'].append(seeder)
        packet = enet.Packet(struct.pack("<I I", port, player_id), enet.PACKET_FLAG_RELIABLE)
        e.peer.send(0, packet)
        logging.info("Seeder linked.")
        players_connected(game_name)

      # -- leech game
      elif t == AW_cmd_type_leech_game:
        t, game_name, player_id = struct.unpack_from("<I 64s I", e.packet.data) 
        game_name = game_name[:game_name.index('\x00')+1]
        games[game_name]['leechers'].append((e.peer, player_id))
        logging.info("Leecher linked.")
        #logging.info(games[game_name])
        players_connected(game_name)
      else:
        logger.info("message type unknown")
    master_host.flush()

# -- terminate threads
"""input_thread.join()"""
      
    