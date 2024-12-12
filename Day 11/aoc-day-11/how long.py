import numpy as np
import matplotlib.pyplot as plt

# Provided data
blinks = np.array([38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49])
times = np.array([0.8471387, 1.2949679, 1.9411298, 2.9861972, 4.5101009, 7.1714607, 11.0322145, 16.59491, 25.3663579, 36.3075178, 54.5979093, 83.796631])

# Fit an exponential curve
log_times = np.log(times)
coefficients = np.polyfit(blinks, log_times, 1)
a, b = coefficients
estimated_times = np.exp(a * blinks + b)

# Predict for blink 75
blink_75_time = np.exp(a * 75 + b)
print(f"Estimated time for blink 75: {blink_75_time:.2f} seconds")

# Plotting for visualization
plt.plot(blinks, times, 'o', label='Original Data')
plt.plot(blinks, estimated_times, '-', label='Fitted Exponential Curve')
plt.axvline(x=75, color='r', linestyle='--', label='Blink 75')
plt.xlabel('Blink')
plt.ylabel('Time (s)')
plt.legend()
plt.show()
