# Drawing with Space Filling Curves

### Do something like

![Peano process](http://41.media.tumblr.com/011be9bcc53483d97eb8c2a3aba77ada/tumblr_nld0lpScmZ1uq1d8po9_r1_1280.png)

### and draw it like

![Peano drawing](http://41.media.tumblr.com/99fadfd3d73d4805435b92a1527bd9cf/tumblr_nld0lpScmZ1uq1d8po4_1280.jpg)

## Usage

If you happen to have an HPGL plotter hanging around, you should be able to use it, just change the serial port in `Plotter.cpp`.

The parameters are at the top of `ofApp.cpp` (bad habit) and there are a few in `Plotter.cpp` that you might want tune for the size of the drawing.

## Thanks

I got all my L-system rules from here:

(http://www-user.uni-bremen.de/~schmuhl/fractals/fractal_curves.html)[http://www-user.uni-bremen.de/~schmuhl/fractals/fractal_curves.html]


### Addons

You will need ofxCv and ofxPosterize
