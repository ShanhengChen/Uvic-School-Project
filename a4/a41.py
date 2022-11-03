#!/usr/bin/env python3

from typing import IO

'''
initializes Circle x,y coordinate and Circle radius by the first input tuple
initializes Circle red,blue,green color and shape opacity by the second input tuple

'''
class Circle:
    def __init__(self, cir: tuple, col: tuple):
        self.cx: int = cir[0]
        self.cy: int = cir[1]
        self.rad: int = cir[2]
        self.red: int = col[0]
        self.green: int = col[1]
        self.blue: int = col[2]
        self.op: float = col[3]

'''
initializes Rectangle x,y coordinate and Rectangle width,height by the first input tuple
initializes Rectangle red,blue,green color and shape opacity by the second input tuple

'''
class Rectangle:
    def __init__(self, rec: tuple, col: tuple):
        self.cx: int = rec[0]
        self.cy: int = rec[1]
        self.w: int = rec[2]
        self.h: int = rec[3]
        self.red: int = col[0]
        self.green: int = col[1]
        self.blue: int = col[2]
        self.op: float = col[3]

'''
initializes ProEpilogue x,y coordinate and Ellipse x,y radius by the first input tuple
initializes ProEpilogue red,blue,green color and shape opacity by the second input tuple

'''
class ProEpilogue:
    def __init__(self, epil: tuple, col:tuple) -> None:
        self.ex: int = epil[0]
        self.ey: int = epil[1]
        self.rx: int = epil[2]
        self.ry: int = epil[3]
        self.red: int = col[0]
        self.green: int = col[1]
        self.blue: int = col[2]
        self.op: float = col[3]
        
def writeHTMLcomment(f: IO[str], t: int, com: str):
    '''writeHTMLcomment method'''
    ts: str = "   " * t
    f.write(f'{ts}<!--{com}-->\n')
   
def drawProEpilogueLine(f: IO[str], t: int, epil: ProEpilogue):
    '''drawProEpilogue method'''
    ts: str = "   " * t
    line: str = f'<ellipse cx="{epil.ex}" cy="{epil.ey}" rx="{epil.rx}" ry="{epil.ry}" fill="rgb({epil.red}, {epil.green}, {epil.blue})" fill-opacity="{epil.op}"></ellipse>'
    f.write(f"{ts}{line}\n")

def drawRectangleLine(f: IO[str], t: int, rec: Rectangle):
    '''drawRectangle method'''
    ts: str = "   " * t
    line: str = f'<rect x="{rec.cx}" y="{rec.cy}" width="{rec.w}" height="{rec.h}" fill="rgb({rec.red}, {rec.green}, {rec.blue})" fill-opacity="{rec.op}"></rect>'
    f.write(f"{ts}{line}\n")

def drawCircleLine(f: IO[str], t: int, c: Circle):
    '''drawCircle method'''
    ts: str = "   " * t
    line: str = f'<circle cx="{c.cx}" cy="{c.cy}" r="{c.rad}" fill="rgb({c.red}, {c.green}, {c.blue})" fill-opacity="{c.op}"></circle>'
    f.write(f"{ts}{line}\n")
       
'''
draw five circles for the first row 
draw five rectangle for the second row
draw five ellipse for the thrid row
   
'''
def genArt(f: IO[str], t: int):
   drawCircleLine(f, t, Circle((50,50,25), (215,95,135,0.3)))
   drawCircleLine(f, t, Circle((150,50,25), (215,95,175,0.4)))
   drawCircleLine(f, t, Circle((250,50,25), (215,95,215,0.5)))
   drawCircleLine(f, t, Circle((350,50,25), (215,135,135,0.6)))
   drawCircleLine(f, t, Circle((450,50,25), (215,135,255,0.7)))

   drawRectangleLine(f, t, Rectangle((25,150,50,10), (0,255,95,0.6)))
   drawRectangleLine(f, t, Rectangle((125,150,50,10), (0,255,135,0.7)))
   drawRectangleLine(f, t, Rectangle((225,150,50,10), (0,255,175,0.8)))
   drawRectangleLine(f, t, Rectangle((325,150,50,10), (0,255,215,0.9)))
   drawRectangleLine(f, t, Rectangle((425,150,50,10), (0,255,255,1.0)))

   drawProEpilogueLine(f, t, ProEpilogue((50,250,20,20), (255,215,0,0.6)))
   drawProEpilogueLine(f, t, ProEpilogue((150,250,20,17), (255,215,95,0.5)))
   drawProEpilogueLine(f, t, ProEpilogue((250,250,20,15), (255,215,135,0.4)))
   drawProEpilogueLine(f, t, ProEpilogue((350,250,20,13), (255,215,175,0.3)))
   drawProEpilogueLine(f, t, ProEpilogue((450,250,20,11), (255,215,215,0.2)))
   
        
def openSVGcanvas(f: IO[str], t: int, canvas: tuple):
     '''openSVGcanvas method'''
     ts: str = "   " * t
     writeHTMLcomment(f, t, "Define SVG drawing box")
     f.write(f'{ts}<svg width="{canvas[0]}" height="{canvas[1]}">\n')

def closeSVGcanvas(f: IO[str], t: int):
    '''closeSVGcanvas method'''
    ts: str = "   " * t
    f.write(f'{ts}</svg>\n')
    f.write(f'</body>\n')
    f.write(f'</html>\n')

def writeHTMLline(f: IO[str], t: int, line: str):
     '''writeLineHTML method'''
     ts = "   " * t
     f.write(f"{ts}{line}\n")

def writeHTMLHeader(f: IO[str], winTitle: str):
    '''writeHeadHTML method'''
    writeHTMLline(f, 0, "<html>")
    writeHTMLline(f, 0, "<head>")
    writeHTMLline(f, 1, f"<title>{winTitle}</title>")
    writeHTMLline(f, 0, "</head>")
    writeHTMLline(f, 0, "<body>")

def writeHTMLfile():
    '''writeHTMLfile method'''
    fnam: str = "myPart1Art.html"
    winTitle = "My Art"
    f: IO[str] = open(fnam, "w")
    writeHTMLHeader(f, winTitle)
    openSVGcanvas(f, 1, (500,300))
    genArt(f, 2)
    closeSVGcanvas(f, 1)
    f.close()

def main():
    '''main method'''
    writeHTMLfile()

if __name__ == "__main__":
    main()

                                                                                                                                                                                                                                                                                                        
