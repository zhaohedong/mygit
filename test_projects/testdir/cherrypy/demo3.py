import cherrypy
 
class HelloWorld:
    @cherrypy.expose
    def hello(self,firstname,lastname):
        return "i love you"+" : "+firstname+lastname
 
    @cherrypy.expose
    def default(self, year, month, day,aa):
        return "error";
    @cherrypy.expose
    def my111(self):
        return "my111";
cherrypy.quickstart(HelloWorld())
