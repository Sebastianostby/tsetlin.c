from sklearn.datasets import fetch_openml
import numpy as np
import struct
import os

def get_mnist():
    print("Fetching MNIST dataset from OpenML...")
    X, y = fetch_openml('mnist_784', version=1, return_X_y=True, as_frame=False)
    print("Dataset fetched. Binarizing images...")
    X = np.where(X > 75, 1, 0).astype(np.uint8)
    y = y.astype(np.uint32)
    print(f"Images and labels prepared. Dataset shape: X={X.shape}, y={y.shape}")
    return X[:60000], X[60000:], y[:60000], y[60000:]

def save_binary_dataset(X, y, filename):
    print(f"Saving {X.shape[0]} samples to {filename}...")
    with open(filename, 'wb') as f:
        for i in range(X.shape[0]):
            f.write(X[i].tobytes())              # 784 bytes
            f.write(struct.pack('I', y[i]))      # 4 bytes
    print(f"Saved {filename} successfully.")

if __name__ == "__main__":
    X_train, X_test, y_train, y_test = get_mnist()
    os.makedirs('data/RAW_FILES/', exist_ok=True)
    save_binary_dataset(X_train, y_train, 'data/RAW_FILES/mnist_train.bin')
    save_binary_dataset(X_test, y_test, 'data/RAW_FILES/mnist_test.bin')
    print("All files saved. Process completed.")
