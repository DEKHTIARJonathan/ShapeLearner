--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
                            ____  _   _  ____ 
                           (_  _)( )_( )( ___)
                             )(   ) _ (  )__)  
                            (__) (_) (_)(____)  
    ___  ____  __  __  ___     __    ____  ____  ____    __    ____  _  _ 
   / __)(_  _)(  \/  )/ __)   (  )  (_  _)(  _ \(  _ \  /__\  (  _ \( \/ )
  ( (__  _)(_  )    (( (_-.    )(__  _)(_  ) _ < )   / /(__)\  )   / \  / 
   \___)(____)(_/\/\_)\___/   (____)(____)(____/(_)\_)(__)(__)(_)\_) (__) 


                  The C++ Template Image Processing Library

                        ( http://cimg.sourceforge.net )

                                  v.1.1.5

--------------------------------------------------------------------------------

# Summary
#---------

  The CImg Library is an open-source C++ toolkit for image processing.
  It consists in a single header file 'CImg.h' providing a set of C++ classes
  and functions that can be used in your own sources, to load/save, process
  and display images. Very portable (Unix/X11,Windows, MacOS X, FreeBSD,..),
  efficient, easy to use, it's a pleasant toolkit for coding image processing
  stuffs in C++.
  
# Author
#--------

  David Tschumperle  ( http://www.greyc.ensicaen.fr/~dtschump/ )

  with the help of various contributors, including :

  - Haz-Edine Assemlal
  - Jerome Boulanger
  - Eric Fausett
  - Sebastien Fourey
  - Jean-Daniel Guyot
  - Matt Hanson
  - Christoph Hormann
  - Werner Jainek
  - Pierre Kornprobst
  - Francois Lauze
  - Thomas Martin
  - Renaud Peteri
  - Konstantin Spirin
  - Thierry Thomas
  - Yu-En-Yun
  - Bug Zhao

# Institution
#-------------

 GREYC Image / CNRS UMR 6072 / FRANCE

 The CImg Library project started during the year 2000, at the INRIA-Sophia
 Antipolis/France (http://www-sop.inria.fr/) when I did my PhD thesis.
 Since October 2004, it is maintained and developed in the Image group of
 the GREYC Lab (CNRS, UMR 6072), in Caen/France	    
 Home page of the team : http://www.greyc.ensicaen.fr/EquipeImage/

# License
#---------

 The source code of the CImg Library is distributed under two different licenses :

 - The main library file 'CImg.h' is distributed under the CeCiLL-C license
   (file 'Licence_CeCILL-C_V1-en.txt').
   This License is a Free-Software license, adapted to the distribution of
   library components, and is close in its terms to the LGPL license.

 - All other files are distributed under the CeCiLL license
   (file 'Licence_CeCILL_V2-en.txt').
   This License is a Free-Software license, compatible with the GPL license.

 These two CeCiLL licenses ( http://www.cecill.info/index.en.html ) have been
 created under the supervision of the three biggest research institutions on
 computer sciences in France :

   - CNRS  ( http://www.cnrs.fr/ )
   - CEA   ( http://www.cea.fr/ )
   - INRIA ( http://www.inria.fr/ )

 You have to RESPECT these licenses. More particularly, please carefully read
 the license terms before using the CImg library in commercial products.

# Package structure :
#--------------------

  The directory CImg/ is organized as follows :

  - CImg.h                     : The single (header) file of the library itself.
  - CHANGES.txt                : A list of changes between CImg versions.
  - Licence_CeCILL-C_V1-en.txt : The CeCiLL-C license governing the use of CImg.h.
  - Licence_CeCILL_V2-en.txt   : The CeCiLL license governing the use of other CImg files.
  - README.txt                 : This file.

  - documentation/ : Directory containing a copy of the CImg web page in html
                     format. The documentation has been generated
		     automatically with the tool 'doxygen' (www.doxygen.org).
		     Note that you can also access the documentation online at
		     http://cimg.sourceforge.net/
                     
  - compilation/   : Directory containing several sub-directories that provide
                     project files or scripts to ease the compilation of the
		     examples, with various C++ compilers on Unix and Windows.    

  - examples/      : Directory containing a lot of example programs performing
                     various stuffs, based on the CImg library, including :

   + CImg_demo.cpp :
   +---------------
      A multi-part demo of the CImg capabilities
     (should be tested first to get an overview).

   + cimgmatlab_cannyderiche.cpp :
   +-----------------------------
      Show how to use the cimgmatlab plugins for integration of CImg-based
      functions in Matlab.

   + fade_images.cpp :
   +-----------------
      Perform an oriented fading between two images.

   + greysctoration.cpp :
   +--------------------
      An implementation of the GREYCstoration algorithm for
      image denoising and interpolation.
      ( http://www.greyc.ensicaen.fr/~dtschump/greycstoration/ )

   + greysctoration4integration.cpp :
   +--------------------------------
      A special version of GREYCstoration for easy integration in
      third parties open source softwares.

   + hough_transform.cpp :
   +---------------------
      Implementation of the Hough Transform to detect lines in images.

   + inrcast.cpp :
   +-------------
       Tool used to convert 2D/3D images with command line arguments.

   + odykill.cpp :
   +-------------
      A simple shoot-em-up game, featuring the team of the Odyssee Lab.

   + mcf_levelsets.cpp :
   +-------------------
      Evolution of a curve, using level sets and 2D distance functions.

   + mcf_levelsets3D.cpp :
   +---------------------
      Evolution of a surface, using level sets and 3D distance functions.

   + image_registration.cpp :
   +------------------------
      Register two images and do an automatic morphing between them.

   + image2ascii.cpp :
   +-----------------
      A correlation-based converter of images to ASCII-art.

   + image_surface.cpp :
   +-------------------
      A tool to view any image as a surface in 3D.

   + pde_heatflow2D.cpp :
   +-------------------
      An implementation of the Heat Flow PDE applied on a 2D image.

   + pde_TschumperleDeriche2D.cpp :
   +------------------------------
      An implementation of the restoration/inpainting PDE,
      as described in Tschumperle-Deriche:CVPR'03 (monoscale).

   + pslider.cpp :
   +-------------
      A tool to generate and view presentations from Postscript files.

   + tetris.cpp :
   +------------
      An implementation of the well-known tetris game in few lines.

   + tutorial.cpp :
   +-------------
      A very simple code to get started with the CImg library.

   + wavelet_atrous.cpp :
   +--------------------
      An example of wavelet transform.


# Getting started
#-----------------
 
  First, you should try to compile the different examples provided in the
  'examples/' directory. Look at the 'compilation/' directory to ease this
  compilation step on different plateforms. Then, you can look at the
  documentation 'documentation/head.html' and the complete reference
  'documentation/reference/index.html' to learn more about CImg functions and
  classes. Finally, you can participate to the 'Forum' section of the
  CImg web page and ask for help if needed.

------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------
