# Library System in Python
# By- PedisettiVenkatesh

from ast import While
from distutils.log import error
from math import floor
import os
from random import choice
import re
import sys
import numpy as np

symbol = '·'
col, row = os.get_terminal_size()
if col < 100:
    print('This Program is better viewed in terminal size greater than 100.\nPlease ReSize the terminal and run again')
    exit()
elif col % 2 == 1:
    window_length = col-1
else:
    window_length = col


def cls():
    """
    Clear the console screen
    """
    os.system('cls' if os.name == 'nt' else 'clear')

#  TODO


def get_int_choice():
    choice = '0'
    while type(choice) is not int:
        choice = input(symbol+'  Your Choice : ')


def get_choice():
    choice = input(symbol+'  Your Choice : ')
    if type(choice) is not int:
        print

    return choice


def line():
    print(symbol*window_length)


def space_line():
    print(symbol+' '*(window_length-2)+symbol)


def centered_text(text):
    print(symbol+' '*int((window_length - len(text))/2-1) +
          text+' '*int((window_length - len(text))/2-1)+symbol)


def lefted_text(text):
    print(symbol+text+' '*int((window_length - len(text)-2))+symbol)


def welcome_screen():
    line()
    space_line()
    centered_text('Library System In Python')
    space_line()
    line()
    space_line()
    lefted_text('  Enter the values repectively...')
    centered_text('1  Search Book')
    centered_text('2  Login      ')
    centered_text('3  Register   ')
    centered_text('4  Exit       ')
    space_line()
    line()

    choice = 0

    while choice not in [1, 2, 3, 4]:
        choice = get_choice()
        if choice not in [1, 2, 3, 4]:
            print('wrong choice try again.')
    cls()
    return choice


cls()
print(welcome_screen())


class Book():
    def __init__(self, Name, Author, Id):
        self.name = Name
        self.author = Author
        self.id = Id

    def borrow():
        pass
