from sprite import *

import pygame
import sys
import os
import signal



"""

ENSICAEN
6 Boulevard Maréchal Juin
F-14050 Caen Cedex
 
This file is owned by ENSICAEN students. No portion of this
document may be reproduced, copied or revised without written
permission of the authors.

Author: Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>

"""


class Interface:

    WIDTH, HEIGHT = (1600, 900)
    TITLE = "Epidemic Simulator"
    BACKGROUND_COLOR = (30, 30, 30)
    PAUSE_COLOR = (14, 168, 23)
    RUN_COLOR = (222, 114, 95)
    FPS = 30

    def __init__(self, memoryShared):
        self.screen = pygame.display.set_mode((self.WIDTH, self.HEIGHT))
        pygame.display.set_caption(self.TITLE)
        self.sprites = pygame.sprite.Group()
        self.memoryShared = memoryShared
        self.clock = pygame.time.Clock()
        self.pid = []
        self.pause = True
        hitboxButton = pygame.Rect(980, 655, 210, 55)
        self.runButton = Button(hitboxButton, self.getTextButton, self.switchState, self.getColorRun)

    def getColorRun(self):
        if self.pause:
            return self.PAUSE_COLOR
        return self.RUN_COLOR
        
    def getTextButton(self):
        if self.pause:
            return "Run ▶"
        return "Stop ■"

    def setCity(self, city):
        hitboxDay = pygame.Rect(1050, 535, 250, 90)
        self.city = city
        self.day = TextBox(hitboxDay, city.getTextDay, fontsize = 50)

    def setPressAgency(self, pressAgency):
        self.pressAgency = pressAgency
        hitboxButton = pygame.Rect(980, 720, 210, 55)
        self.realityButton = Button(hitboxButton, pressAgency.getTextRealTime,
                                    pressAgency.setStateReality, pressAgency.getColorRealTime, 20)
        hitboxButton = pygame.Rect(980, 785, 210, 55)
        self.pressAgencyButton = Button(hitboxButton, pressAgency.getTextPressAgency,
                                        pressAgency.setStatePress, pressAgency.getColorPressAgency, 20)

    def setCitizenManager(self, citizenManager):
        self.citizenManager = citizenManager

    def setInfo(self, info):
        self.info = info

    def switchState(self):
        if self.pause:
            for pid in self.pid: os.kill(pid, signal.SIGCONT)
        else:
            for pid in self.pid: os.kill(pid, signal.SIGTSTP)
        self.pause = not self.pause

    def loop(self):
        self.running = True
        while self.running:
            
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = False
                if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                    self.runButton.onClick()
                    self.realityButton.onClick()
                    self.pressAgencyButton.onClick()
            self.update()
            self.draw()
            if Button.HAND:
                pygame.mouse.set_cursor(pygame.SYSTEM_CURSOR_HAND)
            else:
                pygame.mouse.set_cursor(pygame.SYSTEM_CURSOR_ARROW)
            Button.HAND = False
            pygame.display.flip()
            self.clock.tick(self.FPS)
        self.exit()

    def update(self):
        self.memoryShared.update(self, self.city, self.citizenManager)

    def draw(self):        
        self.screen.fill(self.BACKGROUND_COLOR)
        self.city.draw(self.screen)
        self.pressAgency.draw(self.screen)
        self.citizenManager.draw(self.screen)
        self.runButton.draw(self.screen)
        self.realityButton.draw(self.screen)
        self.pressAgencyButton.draw(self.screen)
        self.day.draw(self.screen)
        self.info.draw(self.screen)

    def exit(self):
        self.memoryShared.writeEndOfSimulation()
        for pid in self.pid: os.kill(pid, signal.SIGUSR2)
        pygame.quit()
        sys.exit()
