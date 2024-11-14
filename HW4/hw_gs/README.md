# HW_Gaussian

## Execution
python train.py --single_image_fitting
python train.py


## GS package
git clone https://github.com/nerfstudio-project/gsplat.git --recursive
cd gsplat
module load cuda/12.1.
module load gcc/10.3.0
python -m pip install -e .
python -m pip install -r examples/requirements.txt
python examples/datasets/download_dataset.py

## SSH Forwarding
- On the local machine, run:
```
ssh -L 8090:localhost:8090 nechy@greatlakes.arc-ts.umich.edu
```
- On the login node, run:
```
ssh -L 8090:localhost:8090 gl1020.arc-ts.umich.edu
```
gl1514.arc-ts.umich.edu

- On the Computing Node, run:
```
cd examples/

python simple_trainer.py default \
    --data_dir data/360_v2/garden/ --data_factor 4 \
    --result_dir ./results/garden--port $8090 --max_steps 20000
```