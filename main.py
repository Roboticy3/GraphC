import sqlite3
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Connect to the SQLite database
conn = sqlite3.connect("out.db")
cursor = conn.cursor()

# Fetch all columns in the table
cursor.execute("PRAGMA table_info(samples)")
columns = [col[1] for col in cursor.fetchall()]

# Separate columns by category
order_columns = [col for col in columns if col.startswith("order")]
edge_prob_columns = [col for col in columns if col.startswith("edge_probability")]
edges_columns = [col for col in columns if col.startswith("edges")]

# Query the data and calculate averages for each category
cursor.execute("SELECT * FROM samples")
rows = cursor.fetchall()
conn.close()

# Convert rows to a NumPy array for easier manipulation
data = np.array(rows)

# Calculate averages across columns in each category
order_avg = np.mean(data[:, [columns.index(col) for col in order_columns]], axis=1)
edge_prob_avg = np.mean(data[:, [columns.index(col) for col in edge_prob_columns]], axis=1)
edges_avg = np.mean(data[:, [columns.index(col) for col in edges_columns]], axis=1)

# Create the 3D plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
sc = ax.scatter(order_avg, edge_prob_avg, edges_avg, c=edges_avg, cmap="viridis")

# Set labels
ax.set_xlabel("Order (Average)")
ax.set_ylabel("Edge Probability (Average)")
ax.set_zlabel("Edges (Average)")
plt.colorbar(sc, label="Edges (Average)")

# Show the plot
plt.savefig("3d_point_graph.png", format="png", dpi=300)
