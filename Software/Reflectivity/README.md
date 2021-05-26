## Using RefNX software for modelling data

Refnx is a powerful Python package  that has been used to model and fit the Neutron Reflectometry data obtained with Ofelia and Nellie [[1]](#1).

In our experiments, a Mixed Reflectivity model has been used following the provided guidelines for fitting the data using a Jupyter Notebook script from RefNX. Additional information on how to operate with RefNX can be found elsewhere [[2]](#2).  Here, a brief description of a fitted reflectivity experiment performed using Ofelia is presented:

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

6. Modelling the sample using layers ("snabs") and Mixed Reflectivity. Depending on the data available, several fittings can be made. For example, if data for the unconfined sample, substrate or polysterene is provided, the Mixed Reflectivity will be easier using the results from those fits.

#Si block D2O

sio2_slab = sio2(13.8936, 2.19514)
sio2_slab.thick.setp(vary=False, bounds=(13.8, 14))
sio2_slab.thick.name = 'sio2 thickness'
sio2_slab.rough.setp(vary=False, bounds=(1.9, 8))
sio2_slab.rough.name = name='sio2 roughness'
sio2_slab.vfsolv.setp(0.0, vary=False, bounds=(0.001, 0.4))
sio2_slab.vfsolv.name = 'sio2 solvation'

silanes_slab = silanes(23.4608 ,10.8417)
silanes_slab.thick.setp(vary=False, bounds=(22, 24))
silanes_slab.thick.name = 'silanes thickness'
silanes_slab.rough.setp(vary=False, bounds=(1, 12))
silanes_slab.rough.name = name='silanes/sio2 roughness'
silanes_slab.vfsolv.setp(0.188672, vary=False, bounds=(0.01, 0.3))
silanes_slab.vfsolv.name = 'silanes solvation'

solv_roughness1 = Parameter(1.762616, 'solvent roughness d2o')
solv_roughness1.setp(vary=True, bounds=(1, 10))

s_d2oblock = si | sio2_slab | silanes_slab | d2o(0, solv_roughness1)
s_d2oblock.contract = 1.5

model_d2oblock = ReflectModel(s_d2oblock,scale=0.979909,dq_type = 'pointwise')
model_d2oblock.scale.setp(bounds=(0.9, 1.4), vary=False)
model_d2oblock.bkg.setp(8.17618e-07,bounds=(4e-8, 6e-6), vary=False)
#model_d2omucins.dq.setp(4.6689,bounds=(1, 12), vary=True)

objective_d2oblock = Objective(model_d2oblock, data_d2oblock,use_weights = True)

#Si block with mucins and D2O

mucinsd2o_slab = mucinsd2o(441.043, 6.17033)
mucinsd2o_slab.thick.setp(vary=False, bounds=(400, 500))
mucinsd2o_slab.thick.name = 'mucins thickness d2o'
mucinsd2o_slab.rough.setp(vary=False, bounds=(0.001, 25))
mucinsd2o_slab.rough.name = name='mucins/silanes roughness d2o'
mucinsd2o_slab.vfsolv.setp(0.924083, vary=False, bounds=(0.6, 0.95))
mucinsd2o_slab.vfsolv.name = 'mucins solvation d2o'

solv_roughness2 = Parameter(194.54, 'mucins/solvent roughness d2o')
solv_roughness2.setp(vary=False, bounds=(100, 300))

s_d2omucins = si | sio2_slab | silanes_slab | mucinsd2o_slab | d2o(0, solv_roughness2)
s_d2omucins.contract = 1.5

model_d2omucins = ReflectModel(s_d2omucins,scale=1.0655,dq_type = 'pointwise')
model_d2omucins.scale.setp(bounds=(0.9, 1.4), vary=False)
model_d2omucins.bkg.setp(2.32067e-06,bounds=(4e-12, 1e-5), vary=False)
#model_d2omucins.dq.setp(4.6689,bounds=(1, 12), vary=False)

objective_d2omucins = Objective(model_d2omucins, data_d2omucins,use_weights = True)



#Si block with mucins and H2O

mucinsh2o_slab = mucinsh2o(441.043, 4.07343)
mucinsh2o_slab.thick.setp(vary=True, bounds=(10, 900))
mucinsh2o_slab.thick.constraint = mucinsd2o_slab.thick
mucinsh2o_slab.thick.name = 'mucins thickness d2o'
mucinsh2o_slab.rough.setp(vary=True, bounds=(0.001, 30))
mucinsh2o_slab.rough.constraint = mucinsd2o_slab.rough
#mucinsh2o_slab.rough.name = name='mucins/silanes roughness'
mucinsh2o_slab.vfsolv.setp(0.924083, vary=True, bounds=(0.01, 1.0))
mucinsh2o_slab.vfsolv.constraint = mucinsd2o_slab.vfsolv
#mucinsh2o_slab.vfsolv.name = 'mucins h2o solvation'


#solv_roughness2 = Parameter(97.4674 , 'mucins/solvent roughness')
#solv_roughness2.setp(vary=True, bounds=(0.01, 200))

s_h2omucins = si | sio2_slab | silanes_slab | mucinsh2o_slab |h2o(0, solv_roughness2)

model_h2omucins = ReflectModel(s_h2omucins,scale=1.0655, bkg=2.03462e-06, dq_type ='pointwise')
model_h2omucins.scale.setp(bounds=(0.9, 1.4), vary=False)
model_h2omucins.bkg.setp(bounds=(4e-9, 6e-6), vary=False)
#model_h2omucins.dq.setp(bounds=(4, 6), vary=True)

objective_h2omucins = Objective(model_h2omucins, data_h2omucins,use_weights = True)

global_objective = GlobalObjective([objective_d2oblock,objective_d2omucins,objective_h2omucins])

#hPS

hPS_slab1 = hps(32.9551, 6.44189) 
hPS_slab1.thick.setp(vary=False, bounds=(20, 50))
hPS_slab1.thick.name = 'hPS thickness'
hPS_slab1.rough.setp(vary=False, bounds=(4, 8))
hPS_slab1.rough.name = name='hPS roughness'
hPS_slab1.vfsolv.setp(0.0, vary=False, bounds=(0.0001, 0.4))
hPS_slab1.vfsolv.name = 'hPS solvation'

solv_roughness3 = Parameter(22.7224, 'hPS/Melinex roughness')
solv_roughness3.setp(vary=False, bounds=(1, 29))

s_hPS = air | hPS_slab1 | Melinex (0, solv_roughness3)

model_hPS = ReflectModel(s_hPS,scale=1.01611, bkg=1.18178e-06, dq_type ='pointwise')
model_hPS.scale.setp(bounds=(0.9, 1.4), vary=False)
model_hPS.bkg.setp(bounds=(4e-9, 6e-6), vary=False)
#model_hPS.dq.setp(bounds=(4, 6), vary=True)

objective_hPS = Objective(model_hPS, data_hPS,use_weights = True)

#Confined mucins 1 Bar

sio2_slab = sio2(13.8936, 2.19514)
sio2_slab.thick.setp(vary=False, bounds=(13.8, 14))
sio2_slab.thick.name = 'sio2 thickness'
sio2_slab.rough.setp(vary=False, bounds=(1.9, 8))
sio2_slab.rough.name = name='sio2 roughness'
sio2_slab.vfsolv.setp(0.0, vary=False, bounds=(0.001, 0.4))
sio2_slab.vfsolv.name = 'sio2 solvation'

silanes_slab = silanes(23.4608 ,10.8417)
silanes_slab.thick.setp(vary=False, bounds=(22, 24))
silanes_slab.thick.name = 'silanes thickness'
silanes_slab.rough.setp(vary=False, bounds=(1, 12))
silanes_slab.rough.name = name='silanes/sio2 roughness'
silanes_slab.vfsolv.setp(0.188672, vary=False, bounds=(0.01, 0.3))
silanes_slab.vfsolv.name = 'silanes solvation'

mucinsnopocket_slab = mucinsnopocket(23.9481, 11.7125)
mucinsnopocket_slab.thick.setp(vary=False, bounds=(15, 155))
mucinsnopocket_slab.thick.name = 'mucins no pocket thickness 1bar'
mucinsnopocket_slab.rough.setp(vary=False, bounds=(1, 30))
mucinsnopocket_slab.rough.name = name='mucins/silanes no pocket r 1bar'
mucinsnopocket_slab.vfsolv.setp(0.306441, vary=False, bounds=(0.01, 1.0))
mucinsnopocket_slab.vfsolv.name = 'mucins no pocket solvation'

mucinspocket_slab = mucinspocket(93.4844 , 10.9347)
mucinspocket_slab.thick.setp(vary=False, bounds=(1, 140))
mucinspocket_slab.thick.name = 'mucins pocket thickness'
mucinspocket_slab.rough.setp(vary=True, bounds=(0.001, 30))
mucinspocket_slab.rough.constraint = mucinsnopocket_slab.rough
#mucinspocket_slab.rough.name = name='mucins/silanes pocket roughness'
mucinspocket_slab.vfsolv.setp(0.325543, vary=False, bounds=(0.01, 1.0))
mucinspocket_slab.vfsolv.name = 'mucins pocket solvation'


hPS_slab = hps(32.9551, 21.9099) 
hPS_slab.thick.setp(vary=False, bounds=(20, 80))
hPS_slab.thick.name = 'hPS thickness'
hPS_slab.thick.constraint = hPS_slab1.thick
hPS_slab.rough.setp(vary=False, bounds=(2, 60))
hPS_slab.rough.name = name='hPS roughness'
hPS_slab.vfsolv.setp(0.0, vary=False, bounds=(0.001, 0.4))
hPS_slab.vfsolv.name = 'hPS solvation'

solv_roughness4 = Parameter(12.3995, 'd2o/hPS/mucins roughness')
solv_roughness4.setp(vary=False, bounds=(3, 20))

solv_roughness5 = Parameter(22.6623, 'Melinex/d2o/hPS roughness')
solv_roughness5.setp(vary=False, bounds=(17, 30))

scale1 = Parameter(0.6039300181837898, 'Scale No pockets')
scale1.setp(vary=False, bounds=(0.6,0.72))
scale2 = Parameter(0.007841735744284222, 'Scale pockets')
scale2.setp(vary=False, bounds=(0.001, 1))

s_d2omucinsconfined1 = si | sio2_slab | silanes_slab | mucinsnopocket_slab | hPS_slab | nopockets(0, solv_roughness4)
s_d2omucinsconfined2 = si | sio2_slab | silanes_slab | mucinspocket_slab | pockets(0, solv_roughness5)
s_d2omucinsconfined1.solvent = SLD(6.36 + 0j)
s_d2omucinsconfined2.solvent = SLD(6.36 + 0j)
s_d2omucinsconfined = [s_d2omucinsconfined1,s_d2omucinsconfined2]
scale = [scale1,scale2]

model_d2omucinsconfined = MixedReflectModel(s_d2omucinsconfined, scale, dq_type ='pointwise')
model_d2omucinsconfined.bkg.setp(4.5921055645868026e-07,bounds=(1e-7, 9e-5), vary=False)
#model_d2omucinsconfined.dq.setp(8.027801449301215,bounds=(1, 12), vary=False)

objective_d2omucinsconfined = Objective(model_d2omucinsconfined, data_d2omucinsconfined,use_weights = True)
global_objective = GlobalObjective([objective_d2omucinsconfined ])

8. Fitting procedure

fitter = CurveFitter(global_objective)
fitter.fit('least_squares');
fitter.fit('differential_evolution');

9. Refining through MCMC 

#fitter = CurveFitter(global_objective, nwalkers=200)
#np.random.seed(6)
#fitter.initialise('jitter')
#fitter.reset()

#fitter.sample(400, random_state=1,pool=2);
11. Check results with data and fit plots

## References

<a id="1">[1]</a>  Nelson, A. R., & Prescott, S. W. (2019). refnx: neutron and X-ray reflectometry analysis in Python. Journal of applied crystallography, 52(1), 193-200.

<a id="2">[2]</a>  RefNX software.  Retrieved from https://refnx.readthedocs.io/en/latest/#
