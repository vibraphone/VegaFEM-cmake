===============
VegaFEM Library
===============
:Info: See <http://run.usc.edu/vega/> for more information.
:Revision: 2.0
:Description: Vega FEM is a C++ library for interactive deformable object simulation

.. image:: https://travis-ci.org/vibraphone/VegaFEM-cmake.svg
   :alt: Travis-CI Build Status
   :target: https://travis-ci.org/vibraphone/VegaFEM-cmake

.. image:: https://ci.appveyor.com/api/projects/status/391ybm9hvbj88m23/branch/cmake-osx?svg=true
   :alt: AppVeyor Build Status
   :target: https://ci.appveyor.com/project/vibraphone/vegaFEM-cmake/history

Vega_ is a computationally efficient and stable C/C++ physics library for
three-dimensional deformable object simulation. It is designed to model large
deformations, including geometric and material nonlinearities, and can also
efficiently simulate linear systems. Vega is open-source and free. It is
released under the 3-clause BSD license, which means that it can be used both
in academic research and in commercial applications.

Vega implements several widely used methods for simulation of large
deformations of 3D solid deformable objects:

* co-rotational linear FEM elasticity [MG04]_; it can also compute the exact
  tangent stiffness matrix [Bar12]_ (similar to [CPSS10]_),
* linear FEM elasticity [Sha90]_,
* invertible isotropic nonlinear FEM models [ITF04]_, [TSIF05]_,
* Saint-Venant Kirchhoff FEM deformable models (see [Bar07]_), and
* mass-spring systems.

For any 3D tetrahedral or cubic mesh, Vega can compute both internal elastic
forces and their gradients (tangent stiffness matrix), in any deformed
configuration. Different parts of the mesh can be assigned arbitrary material
properties. Vega can also timestep these models in time under any
user-specified forces, using several provided integrators: implicit backward
Euler [BW98]_, implicit Newmark [Wri02]_, explicit central differences [Wri02]_
and symplectic Euler. All models include support for multi-core computing. In
addition to linear materials, Vega provides neo-Hookean and Mooney-Rivlin
nonlinear material models; arbitrary nonlinear material models can be added to
Vega. For isotropic hyperelastic materials, this is as easy as defining an
energy function, and its first and second derivatives.

Vega is a middleware physics library. It is aimed at researchers and engineers
with some preexisting knowledge in numerical simulation, computer graphics
and/or solid mechanics, who can integrate Vega into their projects. The
strength of Vega lies in its many C/C++ libraries which depend minimally on
each other, and are in most cases independently reusable. Vega contains about
50,000 lines of C/C++ code.

Most of Vega was written by Jernej Barbič.
Other code contributors include Fun Shing Sin, Daniel Schroeder and Christopher Twigg.
Vega is named in honor of `Jurij Vega`_.

.. _Vega:  http://run.usc.edu/vega/
.. _Jurij Vega: http://en.wikipedia.org/wiki/Jurij_Vega
.. [Bar07]  Jernej Barbič.
            Real-time Reduced Large-Deformation Models and Distributed Contact for Computer Graphics and Haptics.
            PhD thesis, Carnegie Mellon University, August 2007.
.. [Bar12]  Jernej Barbič.
            Exact Corotational Linear FEM Stiffness Matrix.
            Technical report, University of Southern California, 2012.
.. [BW98]   David Baraff and Andrew P. Witkin.
            Large Steps in Cloth Simulation.
            In Proc. of ACM SIGGRAPH 98, pages 43–54, July 1998.
.. [CPSS10] Isaac Chao, Ulrich Pinkall, Patrick Sanan, and Peter Schröder.
            A Simple Geometric Model for Elastic Deformations.
            ACM Transactions on Graphics, 29(3):38:1–38:6, 2010.
.. [ITF04]  G. Irving, J. Teran, and R. Fedkiw.
            Invertible Finite Elements for Robust Simulation of Large Deformation.
            In Symp. on Computer Animation (SCA), pages 131–140, 2004.
.. [MG04]   M. Müller and M. Gross.
            Interactive Virtual Materials.
            In Proc. of Graphics Interface 2004, pages 239–246, 2004.
.. [Sha90]  Ahmed A. Shabana.
            Theory of Vibration, Volume II: Discrete and Continuous Systems. Springer–Verlag, 1990.
.. [TSIF05] Joseph Teran, Eftychios Sifakis, Geoffrey Irving, and Ronald Fedkiw.
            Robust Quasistatic Finite Elements and Flesh Simulation.
            In Symp. on Computer Animation (SCA), pages 181–190, 2005.
.. [Wri02]  Peter Wriggers.
            Computational Contact Mechanics.
            John Wiley & Sons, Ltd., 2002.
