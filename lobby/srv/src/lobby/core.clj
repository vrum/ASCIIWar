;
; ASCIIWar
; Copyright (c) 2014 Frédéric Brachfogel
; All rights reserved.
;
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions are met:
;     * Redistributions of source code must retain the above copyright
;       notice, this list of conditions and the following disclaimer.
;     * Redistributions in binary form must reproduce the above copyright
;       notice, this list of conditions and the following disclaimer in the
;       documentation and/or other materials provided with the distribution.
;     * The name of Frédéric Brachfogel may not be used to endorse or promote products
;       derived from this software without specific prior written permission.
;
; THIS SOFTWARE IS PROVIDED BY FREDERIC BRACHFOGEL ``AS IS'' AND ANY
; EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
; WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
; DISCLAIMED. IN NO EVENT SHALL FREDERIC BRACHFOGEL BE LIABLE FOR ANY
; DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
; (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
; ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
; SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
(defn -main [& args])
(ns lobby.core                                                                                                                                       
  (:import [org.eclipse.jetty.servlet ServletContextHandler ServletHolder]                                                                                                                                  
           org.eclipse.jetty.server.handler.ContextHandlerCollection)
  (:require [compojure.route :as route]))
(use 'ring.adapter.jetty
     'ring.middleware.stacktrace
     'ring.middleware.session
     'ring.util.servlet
     'ring.util.response
     'compojure.core
     '[ring.middleware.json :only [wrap-json-response]]
     '[clojure.java.shell :only [sh]])
;
;
(def release? false)
;
;
(def db-path "db/")
(def user-path (str db-path "user/"))
;
;
(def game-server (atom nil))
(def waiting_players_1v0 (atom (list)))
(def matched_players_1v0 (atom (list)))
(def matcher_1v0 (agent nil))
(def waiting_players_1v1 (atom (list)))
(def matched_players_1v1 (atom (list)))
(def matcher_1v1 (agent nil))
;
;
(defn user-exists? [username]
  (loop [fs (file-seq (clojure.java.io/file user-path))]
    (when (not (empty? fs))
     (if (= username (.getName (first fs)))
       true
       (recur (rest fs))))))
;
;
(defn get-user [username]
  (load-file (str user-path username)))
;
;
(defn user-ok? [username password]
  (= password ((get-user username) :password)))
;
;
(defn user-session-ok? [session username token]
  (when (not (nil? (session :token)))
    (= token (.toString (session :token)))))
;
;
(defroutes handler
  (GET "/register/:username/:password" {{username :username password :password} :params session :session}
    (if (user-exists? username)
      (response {:already-exist? true})
      (let [user-db {:username username :password password}
            f       (str user-path username)
            token   (java.util.UUID/randomUUID)
            session (assoc session :token token)]
        (spit f (pr-str user-db))
        (-> (response {:success true :token token})
            (assoc :session session)))))
  (GET "/login/:username/:password" {{username :username password :password} :params session :session}
    (if (user-exists? username)
      (if (user-ok? username password)
        (let [token   (java.util.UUID/randomUUID)
              session (assoc session :token token)]
          (println (str username " is logged."))
          (-> (response {:success true :token token})
              (assoc :session session)))
        (response {:success false}))
      (response {:success false})))
  (GET "/play1v0/:username/:token" {{username :username token :token} :params session :session}
    (if (user-exists? username)
      (if (user-session-ok? session username token)
        (do
          (swap! waiting_players_1v0 (fn [x] (cons {:username username :token token} x)))
          (response {:success true}))
        (response {:need-login true}))
      (response {:need-register true})))
  (GET "/wait1v0/:username/:token" {{username :username token :token} :params session :session}
    (if (user-exists? username)
      (if (user-session-ok? session username token)
        (let [matched-p (filter #(and (= username (%1 :username)) 
                                      (= token (%1 :token))) 
                                 @matched_players_1v0)]
          (if (not (empty? matched-p))
            (let [matched-p (first matched-p)]
              ; 'p' is matched and is going to play
              ; it is removed from the matched players
              (swap! matched_players_1v0 
                     (fn [l] (filter #(or (not= username (%1 :username)) 
                                          (not= token (%1 :token))) 
                                     l)))
              (response {:success      true 
                         :game-name    (matched-p :game-name)
                         :player-count (matched-p :player-count)
                         :team-count   (matched-p :team-count)}))
            (response {:success false})))
        (response {:need-login true}))
      (response {:need-register true})))
  (GET "/play1v1/:username/:token" {{username :username token :token} :params session :session}
    (if (user-exists? username)
      (if (user-session-ok? session username token)
        (do
          (swap! waiting_players_1v1 (fn [x] (cons {:username username :token token} x)))
          (response {:success true}))
        (response {:need-login true}))
      (response {:need-register true})))
  (GET "/wait1v1/:username/:token" {{username :username token :token} :params session :session}
    (if (user-exists? username)
      (if (user-session-ok? session username token)
        (let [matched-p (filter #(and (= username (%1 :username)) 
                                      (= token (%1 :token))) 
                                 @matched_players_1v1)]
          (if (not (empty? matched-p))
            (let [matched-p (first matched-p)]
              ; 'p' is matched and is going to play
              ; it is removed from the matched players
              (swap! matched_players_1v1 
                     (fn [l] (filter #(or (not= username (%1 :username)) 
                                          (not= token (%1 :token))) 
                                     l)))
              (response {:success      true 
                         :game-name    (matched-p :game-name)
                         :player-count (matched-p :player-count)
                         :team-count   (matched-p :team-count)}))
            (response {:success false})))
        (response {:need-login true}))
      (response {:need-register true}))))
;
;
(def app
  (-> #'handler
      (wrap-stacktrace)
      (wrap-json-response)
      (wrap-stacktrace)
      (wrap-session {:cookie-attrs {:secure release? :max-age (* 3600 24)}})
      (wrap-stacktrace)))
;
;
(if release?
  (defonce server (atom (run-jetty #'app {:configurator (fn [server]
                                                    (doseq [c (.getConnectors server)]
                                                      (when-not (or (nil? c) (instance? org.eclipse.jetty.server.ssl.SslSocketConnector c))
                                                        (.removeConnector server c)))
                                                    server)
                                    :join? false
                                    :ssl? true
                                    :ssl-port 8080
                                    :keystore "keystore.jks"
                                    :key-password (load-file "key.pass")})))
  (defonce server (atom (run-jetty #'app {:join? false :port 8080}))))
(.stop @server)
;
;
(defn match_them1v0 [_]
  (loop []
    (when (not (.isStopped @server))
      ; match players
      ; their are removed from waiting_players_1v0
      (swap! 
        waiting_players_1v0 
        (fn [ps]
          (loop [l ps]
            (if (> (count l) 0)
              (let [p     (first l)
                    new-p (assoc p 
                            :game-name    (java.util.UUID/randomUUID)
                            :team-count   1 
                            :player-count (+ 1 1))]
                (swap! matched_players_1v0 (fn [x] (cons new-p x)))
                (recur (rest l)))
              l))))
      (recur))))
;
;
(defn match_them1v1 [_]
  (loop []
    (when (not (.isStopped @server))
      ; match players
      ; their are removed from waiting_players_1v1
      (swap! 
        waiting_players_1v1 
        (fn [ps]
          (loop [l ps]
            (if (> (count l) 1)
              (let [game-name (java.util.UUID/randomUUID)
                    p1        (first l)
                    new-p1    (assoc p1 
                               :game-name    game-name
                               :team-count   2 
                               :player-count (+ 1 1))
                    p2        (second l)
                    new-p2    (assoc p2 
                               :game-name    game-name
                               :team-count   2 
                               :player-count (+ 1 1))]
                (println (str "matching " (new-p1 :username) " with " (new-p2 :username) "."))
                (swap! matched_players_1v1 (fn [x] (cons new-p1 (cons new-p2 x))))
                (recur (drop 2 l)))
              l))))
      (recur))))
;
;
(defn start [with-gameserver?]
  (when with-gameserver?
    (swap! game-server 
           (fn [_] (.. Runtime (getRuntime) 
                   (exec "python gameserver.py")))))
  (.start @server)
  (send matcher_1v1 match_them1v1)
  (send matcher_1v0 match_them1v0))
;
;
(defn stop []
  (when (not (nil? @game-server))
    (.destroy @game-server))
  (.stop @server))
