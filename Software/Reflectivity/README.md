## Using RefNX software for modelling data

Refnx is a powerful Python package  that has been used to model and fit the Neutron Reflectometry data obtained with Ofelia and Nellie [[1]](#1).

In our experiments, the following schematics has been used through a Jupyter Notebook file. Additional information on how to operate with RefNX can be found elsewhere [[2]](#2). Here, a brief description of a reflectivity experiment performed using Ofelia is presented:

1. Packages needed to load the fitting procedure

```python
%matplotlib inline
import sys
import matplotlib.pyplot as plt
import numpy as np
import os.path
import refnx, scipy
# the analysis module contains the curvefitting engine
from refnx.analysis import CurveFitter, Objective, Parameter, GlobalObjective, Transform
# the reflect module contains functionality relevant to reflectometry
from refnx.reflect import SLD, ReflectModel, Structure, MixedReflectModel
# the ReflectDataset object will contain the data
from refnx.dataset import ReflectDataset
# version numbers used in this analysis
refnx.version.version, scipy.version.version
```

2. Data paths

```python
#Full path, where the data is located
pth = '/data'

#Data files: in our case, 4 columns data have been used, 
#q, R (Reflectivity), error_R, error_q
#Silicon block
data_d2oblock = ReflectDataset(os.path.join(pth,'D2O_block.mft'))
data_d2oblock.name = "d2osiblock"

#Sample in D2O contrast
data_d2osample = ReflectDataset(os.path.join(pth,'Sample_in_D2O.mft'))
data_d2osample.name = "d2oSilanesmucins"

#Sample in H2O contrast
data_h2osample = ReflectDataset(os.path.join(pth,'Sample_in_H2O.mft'))
data_h2osample.name = "h2oSilanesmucins"

#Polysterene characterization
data_hPS = ReflectDataset(os.path.join(pth,'Polysterene_Melinex.mft'))
data_hPS.name = "hPS"

#Pressure 1 bar, sample confined
data_P1_sample_confined = ReflectDataset(os.path.join(pth,'P1_sample_confined.mft'))
data_P1_sample_confined.name = "P1_sample_confined"

#Pressure 2 bar, sample confined
data_P2_sample_confined = ReflectDataset(os.path.join(pth,'P2_sample_confined.mft'))
data_P2_sample_confined.name = "P2_sample_confined"
```

4. SLD definitions

```python

#SLD of Silicon block layers, D2O, H2O and air (theoretical values)
si = SLD(2.07 + 0j)
sio2 = SLD(3.47 + 0j)
d2o = SLD(6.36 + 0j)
h2o = SLD(-0.56 + 0j)
air = SLD(0.0 + 0j)

#SLD of Silanes (theoretical value)
silanes = SLD(0.7+0J)
silanes.real.setp(vary=False, bounds=(0.6, 0.8))
silanes.real.name='silanes SLD'

#SLD of the unconfined sample in D2O. Use "vary = True" if it is unknown.
sldsample_d2o = Parameter(5.55583, 'sld sample d2o')
sldsample_d2o.setp(vary = True, bounds = (5.2, 6.2))

#SLD of the unconfined sample in H2O. Use "vary = True" if it is unknown.
sldsample_h2o = Parameter(5.82485, 'sld sample h2o')
sldsample_h2o.setp(vary = True, bounds = (3.7, 5.9))

#SLD of the confined sample in D2O (in this case, the same as D2O unconfined).
#sample_nopocket refers to the sample properly confined
#sample_pocket refers to the sample that has been confined with dust or any other contamination
#For this experiment, both are considered equal, as not material variation takes place
Sample_nopocket = SLD(sldsample_d2o)
Sample_pocket = SLD(sldsample_d2o)

#SLD for the backing layer of the pockets contamination
pockets = SLD(4.94199 + 0J)
pockets.real.setp(vary=False, bounds=(4.0, 5.0))
pockets.real.name='pocket SLD 1'

#SLD for the backing layer without pockets contamination
nopockets = SLD(2.61373 + 0J)
nopockets.real.setp(vary=False, bounds=(2.2, 3.0))
nopockets.real.name='no pocket SLD 1'

#SLD of the (hydrogenated) polysterene (theoretical value)
hps = SLD(1.412 +0J)
hps.real.setp(vary=True, bounds= (1.2, 2.8))
hps.real.name='sld hps'

#SLD Melinex (already obtained)
Melinex = SLD(2.53323 + 0J)
Melinex.real.setp(vary=True, bounds=(2.4, 2.8))
Melinex.real.name='silanes SLD'
```

6. Modelling the sample using layers ("snabs").
7. Fitting procedure
8. Check results with data and fit plots

## References

<a id="1">[1]</a>  Nelson, A. R., & Prescott, S. W. (2019). refnx: neutron and X-ray reflectometry analysis in Python. Journal of applied crystallography, 52(1), 193-200.

<a id="2">[2]</a>  RefNX software.  Retrieved from https://refnx.readthedocs.io/en/latest/#
