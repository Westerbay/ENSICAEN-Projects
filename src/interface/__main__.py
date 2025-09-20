import pygame
import time


"""

ENSICAEN
6 Boulevard Maréchal Juin
F-14050 Caen Cedex
 
This file is owned by ENSICAEN students. No portion of this
document may be reproduced, copied or revised without written
permission of the authors.

Author: Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>

"""


if __name__ == "__main__":

    pygame.init()
    time.sleep(1)
    
    from interface import Interface
    from memory import MemoryShared
    
    
    memory = MemoryShared()
    interface = Interface(memory)

    from city import City
    from press import PressAgency
    from citizen import CitizenManager
    from info import Info

    city = City()
    interface.setCity(city)
    interface.setPressAgency(PressAgency())
    interface.setCitizenManager(CitizenManager(city))
    interface.setInfo(Info())
    interface.loop()
    
