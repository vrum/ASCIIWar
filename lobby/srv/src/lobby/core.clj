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
     '[ring.middleware.json :only [wrap-json-response]])
;
;
(def release? false)
;
;
(def db-path "db/")
(def user-path (str db-path "user/"))
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
  (= token (session :token)))
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
          (-> (response {:success true :token token})
              (assoc :session session)))
        (response {:success false}))
      (response {:success false})))
  (GET "/play/:username/:token" {{username :username token :token} :params session :session}
    (println username token)
    (if (user-exists? username)
      (if (user-session-ok? session username token)
        (response {:success true})
        (response {:success false}))
      (response {:need-register true}))))
;
;
(def app
  (-> #'handler
      (wrap-stacktrace)
      (wrap-json-response)
      (wrap-stacktrace)
      (wrap-session {:cookie-attrs {:secure true :max-age (* 3600 24)}})
      (wrap-stacktrace)))
;
;
(if release?
  (defonce server (run-jetty #'app {:configurator (fn [server]
                                                    (doseq [c (.getConnectors server)]
                                                      (when-not (or (nil? c) (instance? org.eclipse.jetty.server.ssl.SslSocketConnector c))
                                                        (.removeConnector server c)))
                                                    server)
                                    :join? false
                                    :ssl? true
                                    :ssl-port 8080
                                    :keystore "keystore.jks"
                                    :key-password (load-file "key.pass")}))
  (defonce server (run-jetty #'app {:join? false :port 8080})))
(.stop server)
;
;
(defn -main [& args])
