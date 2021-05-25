## Using RefNX software for modelling data

Refnx is a powerful Python package  that has been used to model and fit the Neutron Reflectometry data obtained with Ofelia and Nellie. 

In our experiments, the following schematics has been used through a Jupyter Notebook file:

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
#SLD of Silicon block layers (theoretical values)
si = SLD(2.07 + 0j)
sio2 = SLD(3.47 + 0j)

#SLD of Silanes (theoretical value)
silanes = SLD(0.7+0J)

#SLD of the unconfined sample in D2O. Use "vary = True" if it is unknown.
sldsample_d2o = Parameter(5.55583, 'sld sample')
sldsample_d2o.setp(vary = True, bounds = (5.2, 6.2))

#SLD of the unconfined sample in H2O. Use "vary = True" if it is unknown.
sldmucinsd2o_h2o = Parameter(5.82485, 'sld mucins d2o')
sldmucinsd2o_h2o.setp(vary = True, bounds = (3.7, 5.9))

#SLD of the confined sample in D2O (same as D2O unconfined).
#
Sample_nopocket = SLD(sldsample_d2o)
Sample_pocket = SLD(sldsample_d2o)


#SLD of the polysterene (theoretical value)
sldhps = Parameter(1.412, 'sld hps')
sldhps.setp(vary = False, bounds = (1.2, 2.8))

air = SLD(0.0 + 0j)
hps = SLD(sldhps)
hps1 = SLD(sldhps)
hps2 = SLD(sldhps)
hps3 = SLD(sldhps)



pockets = SLD(4.94199 + 0J)
nopockets = SLD(2.61373 + 0J)

pockets2= SLD(5.54975 + 0J)
nopockets2 = SLD(2.60686 + 0J)

pockets3= SLD(5.54975 + 0J)
nopockets3 = SLD(2.60686  + 0J)



Melinex = SLD(2.53323 + 0J)

Melinex.real.setp(vary=True, bounds=(2.4, 2.8))
Melinex.real.name='silanes SLD'

d2o = SLD(6.36 + 0j)
h2o = SLD(-0.56 + 0j)

silanes.real.setp(vary=False, bounds=(0.6, 0.8))
silanes.real.name='silanes SLD'

pockets.real.setp(vary=False, bounds=(4.0, 5.0))
pockets.real.name='pocket SLD 1'
nopockets.real.setp(vary=False, bounds=(2.2, 3.0))
nopockets.real.name='no pocket SLD 1'
pockets2.real.setp(vary=False, bounds=(4.6, 6.36))
pockets2.real.name='pocket SLD 2'
nopockets2.real.setp(vary=False, bounds=(2.2, 3.0))
nopockets2.real.name='no pocket SLD 2'
pockets3.real.setp(vary=True, bounds=(4.6, 6.36))
pockets3.real.name='pocket SLD 3'
nopockets3.real.setp(vary=True, bounds=(2.2, 3.0))
nopockets3.real.name='no pocket SLD 3'
```

6. Modelling the sample using layers ("snabs").
7. Fitting procedure
8. Check results with data and fit plots



```python
require 'redcarpet'
markdown = Redcarpet.new("Hello World!")
puts markdown.to_html
puts markdown.to_html
```
