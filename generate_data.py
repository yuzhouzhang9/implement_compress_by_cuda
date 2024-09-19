import os 
import numpy as np
from multiprocessing import Pool

# Generate a 16-float random number
def generator_data(i):
    sz = 10 * 2**30 // 2#10G
    data_float16 = []
    if i % 2 :
        # normal distribution
        data_float16 = np.float16(np.random.rand(sz))
    else:
        # uniform distribution
        data_float16 = np.float16(np.random.uniform(np.finfo(np.float16).min, np.finfo(np.float16).max,sz))
    with open(f'data/data_{i}.npy', 'wb') as f:
        f.write(data_float16.tobytes())

if __name__ == "__main__":
    # creat new dir
    if not os.path.exists('data'):
        os.mkdir('data')
    
    # 
    with Pool(processes=10) as pool:
        pool.map(generator_data,range(10))