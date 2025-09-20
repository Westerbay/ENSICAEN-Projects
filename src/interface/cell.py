import pygame
import random

"""

ENSICAEN
6 Boulevard Maréchal Juin
F-14050 Caen Cedex
 
This file is owned by ENSICAEN students. No portion of this
document may be reproduced, copied or revised without written
permission of the authors.

Author: Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>

"""


class Cell:

    
    TRANS_COLOR = (11, 12, 13)
    BORDER_COLOR = (185, 238, 138)
    TILE = 128
    HEALTHY_TILE = "resources/img/healthy_land.png"
    SICK_TILE = "resources/img/sick_land.png"
    HOSPITAL = "resources/img/hospital.png"
    FIRESTATION = "resources/img/fire_station.png"
    HOUSE = "resources/img/house.png"
    DECO = [
        "resources/img/grass.png",
        "resources/img/tree.png",
        "resources/img/flower.png",
        "resources/img/rock.png",
        "resources/img/bush.png",
        None
    ]
    SICK_RATE = 0.1

    def __init__(self, row, column, gridLength):
        self.row = row
        self.column = column
        self.gridLength = gridLength        
        self.images = None

    def update(self, typeOfCell, contaminationRate, numberOfPeople):
        self.type = typeOfCell  
        self.contaminationRate = contaminationRate
        self.numberOfPeople = numberOfPeople
        if self.images is None: self.image = self.loadImages()

    def draw(self, surface):
        x, y = self.column * self.TILE, self.row * self.TILE
        if self.contaminationRate >= self.SICK_RATE:
            surface.blit(self.images[1], (x, y))
        else:
            surface.blit(self.images[0], (x, y))

    def loadImages(self):
        imageHealthy = pygame.image.load(self.HEALTHY_TILE).convert_alpha()
        imageSick = pygame.image.load(self.SICK_TILE).convert_alpha()
        imageHealthy, dimension = self.loadImage(imageHealthy)
        imageSick, dimension = self.loadImage(imageSick)
        self.images = self.addComponents(imageHealthy, imageSick, dimension)
        
    def loadImage(self, image):        
        dimension = (self.TILE * 3, self.TILE * 3)
        image = pygame.transform.scale(image, dimension)
        surface = pygame.Surface((self.TILE, self.TILE))
        surface.fill(self.TRANS_COLOR)
        surface.set_colorkey(self.TRANS_COLOR)
        row, column, TILE = self.row, self.column, self.TILE
        if row == 0 and column == 0:
            surface.blit(image, (0, 0))
        elif row == 0 and column == self.gridLength - 1:
            surface.blit(image, (-2 * TILE, 0))
        elif row == 0:
            surface.blit(image, (-TILE, 0))
        elif column == 0 and row == self.gridLength - 1:
            surface.blit(image, (0, -2 * TILE))
        elif column == 0:
            surface.blit(image, (0, -TILE))
        elif row == self.gridLength - 1 and column == self.gridLength - 1:
            surface.blit(image, (-2 * TILE, -2 * TILE))
        elif row == self.gridLength - 1:
            surface.blit(image, (-TILE, -2 * TILE))
        elif column == self.gridLength - 1:
            surface.blit(image, (-2 * TILE, -TILE))
        else:
            surface.blit(image, (-TILE, -TILE))
        pygame.draw.rect(surface, self.BORDER_COLOR,(0, 0, self.TILE, self.TILE), 1)
        return surface, (TILE, TILE)

    def addComponents(self, surfaceHealthy, surfaceSick, dimension):
        offsetX, offsetY = 0, -10
        if self.type == 0:   # Land
            deco = random.choice(self.DECO)
            if deco is None: return surfaceHealthy, surfaceSick
            image = pygame.image.load(deco).convert_alpha()
            offsetX = random.randint(-18, 18)
            offsetY = random.randint(-18, 18)
        elif self.type == 1: # Houses
            image = pygame.image.load(self.HOUSE).convert_alpha()
        elif self.type == 2: # Hospital
            image = pygame.image.load(self.HOSPITAL).convert_alpha()
        elif self.type == 3: # Fire Station
            image = pygame.image.load(self.FIRESTATION).convert_alpha()
        image = pygame.transform.scale(image, dimension)
        surfaceHealthy.blit(image, (offsetX, offsetY))
        surfaceSick.blit(image, (offsetX, offsetY))
        return surfaceHealthy, surfaceSick

    def isBuilding(self):
        return self.type != 0

    def __str__(self):
        return str(self.type)
           
