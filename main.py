import sqlite3
from matplotlib.colors import ListedColormap
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib as mpl

order_avg = np.empty((0,), dtype=float)
edge_prob_avg = np.empty((0,), dtype=float)
prop_avg = np.empty((0,), dtype=float)

# Connect to the SQLite database
conn = sqlite3.connect("out.db")
cursor = conn.cursor()

TABLES = ["samples_1", "samples_2", "samples_3", "samples_4", "samples_5", "samples_6", "samples_7", "samples_8", "samples_9"]

for t in TABLES:
  # Fetch all columns in the table
  try:
    cursor.execute(f"PRAGMA table_info({t})")
  except:
    continue # table does not exist, ignore
  columns = [col[1] for col in cursor.fetchall()]

  # Separate columns by category
  order_columns = [col for col in columns if col.startswith("n_")]
  edge_prob_columns = [col for col in columns if col.startswith("p_")]
  prop_columns = [col for col in columns if col.startswith("property_")]

  print(order_columns, edge_prob_columns, prop_columns)

  # Query the data and calculate averages for each category
  try:
    cursor.execute(f"SELECT * FROM({t})")
  except:
    continue # table does not exist, ignore
  rows = cursor.fetchall()

  # Convert rows to a NumPy array for easier manipulation
  data = np.array(rows)

  # Calculate averages across columns in each category

  order_avg = np.concatenate((order_avg, np.mean(data[:, [columns.index(col) for col in order_columns]], axis=1)))
  edge_prob_avg = np.concatenate((edge_prob_avg, np.mean(data[:, [columns.index(col) for col in edge_prob_columns]], axis=1)))
  prop_avg = np.concatenate((prop_avg, np.mean(data[:, [columns.index(col) for col in prop_columns]], axis=1)))
conn.close()

# Create a custom colormap emphasizing the minimum
colors = mpl.colormaps['viridis'](np.linspace(0, 1, 256))  # Start with 'viridis'
colors[0] = [0, 1, 0, 0.01]
custom_cmap = ListedColormap(colors)

# Find the normalized color for edges_avg
min_val = 1
max_val = np.max(prop_avg)
norm = plt.Normalize(vmin=min_val, vmax=max_val)


# Create the 3D plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

sc = ax.scatter(order_avg, edge_prob_avg, prop_avg, s=5, c=prop_avg, cmap=custom_cmap, norm=norm, marker='s')


# Set labels
ax.set_xlabel("Order")
ax.set_ylabel("Edge Probability")
ax.set_zlabel("Components (Sample Mean)")
#plt.colorbar(sc, label="Components (Sample Mean)")

ax.view_init(elev=30, azim=45)

# Show the plot
#plt.show()

#plt.savefig("numcomponents-angle.png", format="png", dpi=300)

ax.view_init(elev=0, azim=0)

#plt.savefig("numcomponents-p.png", format="png", dpi=300)

ax.view_init(elev=0, azim=90)

#plt.savefig("numcomponents-n.png", format="png", dpi=300)

ax.view_init(elev=90, azim=180)

#plt.savefig("numcomponents-np.png", format="png", dpi=300)



from scipy import optimize

# Define the model
def model1(x, a, b, c, d):
    return a * np.exp(b * (x - c)) + d
def model2(x, a, b, c, d):
    return (a + b)/(c * x + d)
model = model2

# Define the custom loss function based on the conditions you want to minimize
def loss(params, x, y, z):
    a, b, c, d = params
    predicted_y = model(x, a, b, c, d)
    
    # Compute the error
    error = 0
    
    for i in range(len(x)):
        if (x[i] < 3): continue
        if z[i] == 1:
            # Penalize when y < predicted_y (for z = 1)
            if y[i] < predicted_y[i]:
                error += (predicted_y[i] - y[i]) ** 2
        elif z[i] > 1:
            # Penalize when y > predicted_y (for z > 1)
            if y[i] > predicted_y[i]:
                error += (predicted_y[i] - y[i]) ** 2
                
    return error

INITIAL_VALUES = [1, 1, 1, 1]

result = optimize.minimize(loss, INITIAL_VALUES, args=(order_avg, edge_prob_avg, prop_avg))

params_old = (1.1990281330209416, 1.1990281330209562, 0.22948226346548167, 1.8078591471289007)
a_old, b_old, c_old, d_old = params_old

a_opt, b_opt, c_opt, d_opt = result.x
print("Optimized parameters:")
print(f"a = {a_opt}, b = {b_opt}, c = {c_opt}, d = {d_opt}")
print(f"Error: {result.fun}")

print(f"Old error: {loss(params_old, order_avg, edge_prob_avg, prop_avg)}")

#print(a_opt - a_old, b_opt - b_old, c_opt - c_old, d_opt - d_old)

x_curve = np.linspace(np.min(order_avg), np.max(order_avg), 100)
y_curve = model(x_curve, a_opt, b_opt, c_opt, d_opt)
y_old = model(x_curve, a_old, b_old, c_old, d_old)



# Plot the fitted curve
#ax.plot(x_curve, y_curve, np.ones_like(x_curve), color='green', label='Fitted curve')
ax.plot(x_curve, y_old, np.ones_like(x_curve), color='red', label='Old Fitted curve')

plt.savefig("numcomponents-r.png")