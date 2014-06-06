(defproject lobby "0.1.0-SNAPSHOT"
  :description "FIXME: write description"
  :url "http://asciiwar.com"
  :license {:name "MIT License"
            :url "http://asciiwar.com"}
  :dependencies [[org.clojure/clojure "1.3.0"]
                 [ring/ring "1.1.6"]
                 [javax.servlet/servlet-api "2.5"]                                                                                                                                                          
																	[org.eclipse.jetty/jetty-server "7.6.1.v20120215"]                                                                                                                                         
																	[org.eclipse.jetty/jetty-servlet "7.6.1.v20120215"]
																	[compojure "1.1.8"]
																	[crypto-password "0.1.3"]
																	[ring/ring-json "0.3.1"]]
  :main lobby.core)
