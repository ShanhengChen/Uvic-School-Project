#!/usr/bin/env python3

from typing import IO

import random

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

'''
SHA calls private function getShapeNum to generate a Kind of shape numeber
X generates a X-coordinate of shape from 0 to 600 (viewpiont size)
Y generates a Y-coordinate of shape from 0 to 600 (viewpiont size) 
RAD generates a circle radius from 0 to 100        (Circle)
RX generates a x-axis radius for Ellipse from 10 to 30 (Ellipse)
RY generates a Y-axis radius for Ellipse from 10 to 30 (Ellipse)
W generates a width for Rectangle from 10 to 100    (Rectangle)
H generates a height for Rectangle from 10 to 100   (Rectangle)
R generates a Red color RGB in range 0 - 255        (color)
G generates a Green color RGB in range 0 - 255      (color)
B generates a Blue color RGB in range 0 - 255       (color)
OP generates a shape opacity in range 0.0 -1.0      (opacity)
'''
class GenRandom:
    def __init__(self):
        self.SHA = self.__getShapeNum__()
        self.X = random.randint(0,600)
        self.Y = random.randint(0,600)
        self.RAD = random.randint(0,100)
        self.RX = random.randint(10,30)
        self.RY = random.randint(10,30)
        self.W = random.randint(10,100)
        self.H = random.randint(10,100)
        self.R = random.randint(0,255)
        self.G = random.randint(0,255)
        self.B = random.randint(0,255)
        self.OP = round(random.uniform(0.0,1.0),1)
    
    '''
    generates a random number from 0,1,3 
    '''
    def __getShapeNum__(self) ->int:
        shape = random.randint(0,3)
        while shape == 2:
            shape = random.randint(0,3)
        return shape

'''
handle info from GenRandom
'''
class ArtConfig:
    '''ArtConfig class'''
    def __init__(self, f: IO[str], t: int):
        self.shape_num = 10
        self.f = f 
        self.t = t
    '''
    generate GenRandom object ten times and 
    print out table with specific format base on the GenRandom object
    '''
    def printTable(self):
        print("%3s %3s %3s %3s %3s %3s %3s"%("CNT","SHA","X","Y","RAD","RX","RY"),end=" ")
        print("%3s %3s %3s %3s %3s %3s"%("W","H","R","G","B","OP"))
        for i in range(self.shape_num):
            self.shape = GenRandom()
            print("%3d %3d %3d %3d %3d %3d %3d"%(i,self.shape.SHA,self.shape.X,self.shape.Y,self.shape.RAD,self.shape.RX,self.shape.RY),end=" ")
            print("%3s %3s %3s %3s %3s %3s"%(self.shape.W,self.shape.H,self.shape.R,self.shape.G,self.shape.B,self.shape.OP))

       
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
       
def genArt(f: IO[str], t: int):

    x = ArtConfig(f,t)
    x.printTable()
     
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
    fnam: str = "myPart3Art.html"
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

                                                                                                                                                                                                                                                                                                        
