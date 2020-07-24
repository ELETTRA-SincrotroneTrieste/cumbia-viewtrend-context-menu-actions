### Adds a "Copy source" action to the right click menu of cumbia-qtcontrols widget supporting contextual menus

cumbia-qtcontrols widgets that use CuContextMenu to provide a right click menu will add a *Copy source* action
that copies the source (or target) of the widget into the *clipboard*.
The source (target) is copied as plain text.

To install the plugin, execute

- qmake
- make
- make install

after checking that the line

```
include(/usr/local/cumbia-libs/include/cumbia-qtcontrols/cumbia-qtcontrols.pri)
```

in the file cumbia-copy-source-context-menu-actions.pro is correct (in the default case,
cumbia-qtcontrols is installed under /usr/local/cumbia-libs)

Have fun.


