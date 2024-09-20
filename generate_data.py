import os
import numpy as np
from multiprocessing import Pool

# Generate a set of random float16 numbers
def generator_data(i):
    sz = 1 * 2**20 // 2  # 100M in bytes, float16 takes 2 bytes each, so the number of elements is 100MB / 2
    data_float16 = []
    
    if i % 2:
        # Normal distribution
        data_float16 = np.float16(np.random.randn(sz))  # Use randn to generate normally distributed data
    else:
        # Uniform distribution
        data_float16 = np.float16(np.random.uniform(np.finfo(np.float16).min, 
                                                    np.finfo(np.float16).max, sz))
         
    
    # Save data as a binary file
    with open(f'data/data{i}.bin', 'wb') as f:
        f.write(data_float16.tobytes())
        print(f"generated data{i}.bin successfully")

if __name__ == "__main__":
    # Create the data directory if it doesn't exist
    os.makedirs('data', exist_ok=True)
    # Use multiprocessing to generate data in parallel
    with Pool(processes=10) as pool:
        pool.map(generator_data, range(10))
