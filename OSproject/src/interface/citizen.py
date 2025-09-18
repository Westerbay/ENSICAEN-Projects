from city import City
from cell import Cell
from press import PressAgency

import random
import pygame


"""

ENSICAEN
6 Boulevard Maréchal Juin
F-14050 Caen Cedex
 
This file is owned by ENSICAEN students. No portion of this
document may be reproduced, copied or revised without written
permission of the authors.

Author: Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>

"""

class CitizenManager:

    NUMBER_CITIZEN = 25
    NUMBER_DOCTOR = 4
    NUMBER_FIREMAN = 6
    NUMBER_REPORTER = 2

    CITIZEN_FOLDER = "resources/img/citizen/"
    DOCTOR_FOLDER = "resources/img/doctor/"
    FIREMAN_FOLDER = "resources/img/fireman/"
    REPORTER_FOLDER = "resources/img/reporter/"

    CITIZEN = 0
    DOCTOR = 1
    FIREMAN = 2
    REPORTER = 3

    def __init__(self, city):
        self.citizens = [Citizen(city, self.CITIZEN_FOLDER) for _ in range(self.NUMBER_CITIZEN)]
        self.doctors = [Doctor(city, self.DOCTOR_FOLDER) for _ in range(self.NUMBER_DOCTOR)]
        self.firemen = [Fireman(city, self.FIREMAN_FOLDER) for _ in range(self.NUMBER_FIREMAN)]
        self.reporters = [Reporter(city, self.REPORTER_FOLDER) for _ in range(self.NUMBER_REPORTER)]
        self.font = pygame.font.Font(None, 22)
        self.icons = {}

    def update(self):
        for reporter in self.reporters:
            if reporter.state < Citizen.DEAD:
                return
        PressAgency.DEAD = True

    def draw(self, surface):
        self.update()
        for citizen in self.citizens:
            citizen.draw(surface)
        for doctor in self.doctors:
            doctor.draw(surface)
        for fireman in self.firemen:
            fireman.draw(surface)
        for reporter in self.reporters:
            reporter.draw(surface)
        for position in City.BUILDINGS:
            surfaces, positions = self.drawBuildingInfo(position)
            for i in range(len(surfaces)):
                surface.blit(surfaces[i], positions[i])

    def drawBuildingInfo(self, position):
        x = (position % City.GRID_LENGTH) * Cell.TILE
        y = (position // City.GRID_LENGTH) * Cell.TILE
        surfaces = []
        nb = len(City.BUILDINGS[position])
        if nb == 1:
            positions = [(x + 34, y)]
        if nb == 2:
            positions = [(x + 3, y), (x + 65, y)]
        if nb == 3:
            positions = [(x + 3, y), (x + 65, y),
                        (x + 34, y + 35)]
        if nb == 4:
            positions = [(x + 3, y), (x + 65, y),
                        (x + 3, y + 35), (x + 65, y + 35)]
        for NPC in City.BUILDINGS[position]:    
            image = pygame.Surface((60, 30))
            image.fill((1, 2, 3))
            image.set_colorkey((1, 2, 3))
            pygame.draw.rect(image, (0, 0, 0), (0, 0, 60, 30), border_radius = 5)
            pygame.draw.rect(image, (255, 255, 255), (2, 2, 56, 26), border_radius = 5)
            if NPC not in self.icons:
                self.icons[NPC] = pygame.image.load(f"{NPC}icon.png").convert_alpha()
            icon = self.icons[NPC]
            image.blit(icon, (5, 2))
            nb = self.font.render(f"{len(City.BUILDINGS[position][NPC])}", False, (0, 0, 0))
            image.blit(nb, (37, 10))
            surfaces.append(image)
        return surfaces, positions 

class Citizen:

    IDLE_UP = "idle_up"
    IDLE_DOWN = "idle_down"
    WALK_UP = "walk_up"
    WALK_DOWN = "walk_down"
    OFFSET_FOOT = 8

    DEAD_PATH = "resources/img/dead.png"
    
    HEALTHY = 0
    SICK = 1
    DEAD = 2
    BURNT = 3
    
    def __init__(self, city, folder, tileSize = (48, 64)):
        self.city = city
        self.folder = folder
        self.tileSize = tileSize
        self.images = self.generateImages()        
        self.position = -1
        self.state = self.HEALTHY
        self.hidden = False
        width, height = self.getWantedSize()
        self.rect = pygame.Rect(0, 0, width, height)
        self.direction = self.IDLE_DOWN
        self.tick = 0
        self.velTick = 0.3
        self.velocity = 0.03
        self.desiredPosition = (0, 0)
        self.vect = (0, 0)
        self.left = False
        self.leftImages = {}
        self.deadIcon = pygame.image.load(self.DEAD_PATH).convert_alpha()

    def getRealHitbox(self):
        return pygame.Rect(32, 36, 36, 56)

    def getWantedSize(self):
        return (96, 128)

    def loadImage(self, state):
        path = f"{self.folder}{state}.png"
        image = pygame.image.load(path).convert_alpha()        
        width, height = image.get_size()
        numberOfSprite = int(width // self.tileSize[0])
        images = []
        for i in range(numberOfSprite):
            surface = pygame.Surface(self.tileSize)
            surface.fill((1, 2, 3))
            surface.set_colorkey((1, 2, 3))
            surface.blit(image, (-self.tileSize[0] * i, 0))
            surface = pygame.transform.scale(surface, self.getWantedSize())
            images.append(surface)
        return images

    def generateImages(self):
        images = {
            self.IDLE_UP : self.loadImage(self.IDLE_UP),
            self.IDLE_DOWN : self.loadImage(self.IDLE_DOWN),
            self.WALK_UP : self.loadImage(self.WALK_UP),
            self.WALK_DOWN : self.loadImage(self.WALK_DOWN)
        }
        return images

    def convertPosition(self, position):
        x = (position % City.GRID_LENGTH)
        y = (position // City.GRID_LENGTH)
        i, j = random.choice(City.POSITIONS[(x, y)])
        City.POSITIONS[(x, y)].remove((i, j))

        x = x * Cell.TILE + i * Cell.TILE / 4
        y = y * Cell.TILE + j * Cell.TILE / 4
        x += (Cell.TILE / 4 - self.rect.width) / 2
        y += (Cell.TILE / 4 - self.rect.height) / 2
        y -= self.OFFSET_FOOT
        return x, y

    def move(self):               
        if (self.rect.x, self.rect.y) == self.desiredPosition:
            if self.vect[1] < 0:
                self.direction = self.IDLE_UP
            else:
                self.direction = self.IDLE_DOWN
            return        
        if self.vect[1] < 0:
            self.direction = self.WALK_UP
        else:
            self.direction = self.WALK_DOWN
        self.left = self.vect[0] < 0
        self.rect.x += self.vect[0] * self.velocity
        self.rect.y += self.vect[1] * self.velocity
        if (self.rect.x - self.desiredPosition[0]) * self.vect[0] > 0:
            self.rect.x, self.rect.y = self.desiredPosition
        if (self.rect.y - self.desiredPosition[1]) * self.vect[1] > 0:
            self.rect.x, self.rect.y = self.desiredPosition

    def reload(self, position, state):
        x, y = self.convertPosition(position)
        self.vect = (x - self.rect.x, y - self.rect.y)
        self.desiredPosition = (x, y)        
        if self.position == -1 or (state == self.DEAD and self.state != state):
            self.rect.x, self.rect.y = self.desiredPosition
        self.position = position
        self.state = state
        
    def update(self):
        self.move()
        self.hidden = self.city.isCellBuilding(self.rect, self.position)
        if self.hidden:
            if self.position not in City.BUILDINGS:
                City.BUILDINGS[self.position] = {}
            if self.folder not in City.BUILDINGS[self.position]:
                City.BUILDINGS[self.position][self.folder] = set()
            City.BUILDINGS[self.position][self.folder].add(self)

    def getImage(self):
        self.tick += self.velTick
        size = len(self.images[self.direction])
        index = int(self.tick)
        if index >= size:
            index = 0
            self.tick = 0
        image = self.images[self.direction][index]
        if self.left:
            if image not in self.leftImages:
                self.leftImages[image] = pygame.transform.flip(image, True, False)
            return self.leftImages[image]
        return image

    def fixHitboxDead(self):
        x, y = self.rect.x, self.rect.y
        width, height = self.deadIcon.get_size()
        x += (self.rect.width - width) / 2
        y += (self.rect.height - height) / 2
        y += self.OFFSET_FOOT
        return (x, y)

    def draw(self, surface):
        if self.state not in [self.DEAD, self.BURNT]:
            self.update()
        if self.hidden and self.state != self.DEAD:
            return
        if self.state == self.BURNT:
            return
        elif self.state == self.DEAD:
            surface.blit(self.deadIcon, self.fixHitboxDead())
        else:            
            surface.blit(self.getImage(), self.rect)

class Doctor(Citizen):

    def __init__(self, city, folder):
        Citizen.__init__(self, city, folder, (32, 32))

    def getWantedSize(self):
        return (64, 64)

class Fireman(Citizen):

    def __init__(self, city, folder):
        Citizen.__init__(self, city, folder, (80, 64))

    def getWantedSize(self):
        return (120, 96)

class Reporter(Citizen):

    def __init__(self, city, folder):
        Citizen.__init__(self, city, folder)
