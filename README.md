Introduction
============

This project consists in a wearable glove controller for computer/user
interaction through hand tilting and fingers movement.

The glove has several sensors attached to it. All these sensors collect data
(such as inclination, movements and finger flexing) and send it to the computer
through a USB connection.  
The artefact makes use of a micro controller, which has both some built-in
sensors and some GPIO (General Purpose Input Output), which allow the external
flexing sensor to be attached.

The final product is intended for the use of developers and creators.  
To make it easier for people to integrate the device in any sort of graphic
application, the controller comes with a library for developers to be downloaded
from my GitHub account, which I wrote and packaged personally. The latter allows
a quick and easy access to the Glove controller and its sensors.

The project stems from the recent and increasing spread of Virtual / Augmented
reality and human-machine interaction. Possibly more and more people will start
considering working with these features.  
The point of the project is not to take advantage of the market’s increasing
demand of these types of controllers, but to support the developer community
with an affordable and 100% open source device to start with.  
From this comes the idea of not simply making the physical product available,
but also to provide a quicker setup that makes it less time consuming to start
developing.

Installing
==========

You can install download the library by forking the following github repository:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
$ git clone https://github.com/FilippoL/GloveControllerExtension.git
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Because the library makes use of python 2.7 make sure you have that installed
before starting the development.  
Once python is installed, make sure to download the following serial library
using pip or from source [here](https://pythonhosted.org/pyserial/):

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
$ pip install pyserial
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

License
=======

You may copy, distribute and modify the software provided that modifications are
described and licensed for free under
[LGPL-3](https://www.gnu.org/licenses/lgpl-3.0.html) . Derivatives works
(including modifications or anything statically linked to the library) can only
be redistributed under LGPL-3, but applications that use the library don't have
to be.
