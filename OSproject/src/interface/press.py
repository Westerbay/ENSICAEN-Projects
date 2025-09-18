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

class PressAgency:

    PRESENTER_PATH = "resources/img/presenter.png"
    REPORTER_PATH = "resources/img/reporter/icon.png"
    PRESENTER_POSITION = (1100, 450)
    REPORTER_POSITION = (1190, 500)
    REPORTER_SIZE = (416, 416)
    GRAPH_PATH = "resources/citizens_situation.png"
    GRAPH_PATH_PRESS = "resources/press_agency.png"
    DEAD_PATH = "resources/img/dead.png"
    GRAPH_POSITION = (920, 20)
    WHITE = (255, 255, 255)
    BACKGROUND_COLOR = (30, 30, 30)
    BLUE = (0, 120, 215)
    DEEP_BLUE = (26, 81, 179)
    GRAPH_HITBOX = (1280, 640, 50, 35)
    
    REALTIME = 0
    PRESSAGENCY = 1
    DEAD = False

    def __init__(self):
        self.imagePresenter = pygame.image.load(self.PRESENTER_PATH).convert_alpha()
        imageReporter = pygame.image.load(self.REPORTER_PATH).convert_alpha()
        self.imageReporter = pygame.transform.scale(imageReporter, self.REPORTER_SIZE)
        imageDead = pygame.image.load(self.DEAD_PATH).convert_alpha()
        self.imageDead = pygame.transform.scale(imageDead, self.REPORTER_SIZE)
        self.previousGraph = None
        self.state = self.REALTIME

    def getColorPressAgency(self):
        if self.state == self.PRESSAGENCY:
            return self.DEEP_BLUE
        return self.BLUE

    def getColorRealTime(self):
        if self.state == self.REALTIME:
            return self.DEEP_BLUE
        return self.BLUE

    def setStateReality(self):
        self.state = self.REALTIME

    def setStatePress(self):
        self.state = self.PRESSAGENCY

    def getTextRealTime(self):
        return "Real Time News"

    def getTextPressAgency(self):
        return "Press Agency News"

    def draw(self, surface):
        try:
            graph = self.loadGraph()
        except:
            if self.previousGraph is not None:
                graph = self.previousGraph
            else:
                return
        surface.blit(graph, self.GRAPH_POSITION)               
        pygame.draw.rect(surface, self.WHITE, self.GRAPH_HITBOX, border_radius = 50)
        pygame.draw.rect(surface, self.BACKGROUND_COLOR, self.GRAPH_HITBOX, 2, border_radius = 50)
        if self.state == self.REALTIME:
            surface.blit(self.imagePresenter, self.PRESENTER_POSITION)
        elif not self.DEAD:
            surface.blit(self.imageReporter, self.REPORTER_POSITION)
        else:
            surface.blit(self.imageDead, self.REPORTER_POSITION) 
        self.previousGraph = graph
        
    def loadGraph(self):
        if self.state == self.REALTIME:
            path = self.GRAPH_PATH
        else:
            path = self.GRAPH_PATH_PRESS        
        image = pygame.image.load(path).convert()
        width, height = image.get_size()
        gap = 10
        surface = pygame.Surface((width + gap, height + gap))
        surface.fill((1, 2, 3))
        surface.set_colorkey((1, 2, 3))
        pygame.draw.rect(surface, self.WHITE, (0, 0, width + gap, height + gap), border_radius = 20)
        surface.blit(image, (gap // 2, gap // 2))
        return surface
