# rfsom-box-gui

RFSOM-BOX-GUI is a qt5 based graphical user interface for the RFSOM. It provides a simple, scriptable way to implement menus, controls and launch external applications. Modifying the json configuration files will change the menu structure without the need to recompile the whole application.

There are two important json files: landing.json and launcher.json. The former provides a landing page, the first page that is displayed when the device is powered on. In this json a number of elements are described - the ip address, the temperature, battery charge. Each of these elements is updated at a timing interval(also described in the json) by running a simple script/bash command. The output of the command is printed on the screen in a user-friendly format.

launcher.json provides a way to configure the menu of the application. Each element of the main menu is described in the json along with the content of each submenu. There are two types of submenus - "list" and "video". The list type dynamically creates a list of interactable elements - such as buttons, editboxes (numbered or enums), checkboxes or simple labels. Each of these elements are updated at runtime by running simple bash scripts with a user defined cyclicity. The list submenu is widely implemented in the project to provide user input on configuration parameters of the RFSOM.

The other type of submenu is the "video" application which runs a script/application and gives that application control of the display. This type of submenu is used to launch the fft plot application, the video streaming or the testmode.
