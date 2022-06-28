
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd



def plot3dTrajectory():

    df = pd.read_csv('stage0.csv')
    x = df['Latitude [deg]'].tolist()
    y = df['Longitude [deg]'].tolist()
    z = df['Altitude [m]'].tolist()
    z[:] = [i/1000 for i in z]

    maxZ = max(z)
    maxX = max(x)
    minZ = min(z)
    minX = min(x)
    maxY = max(y)
    minY = min(y)
    maxXY = max(maxX, maxY)
    minXY = min(minX, minY)


    # Create figure
    fig1 = plt.figure(figsize=(9, 9))
    ax1 = plt.subplot(111, projection="3d")
    ax1.plot(x, y, zs=minZ, zdir="z", linestyle="--")
    ax1.plot(
        x,
        z,
        zs=minY,
        zdir="y",
        linestyle="--",
        )
    ax1.plot(
        y,
        z ,
        zs=minXY,
        zdir="x",
        linestyle="--",
        )
    ax1.plot(
        x, y, z , linewidth="2"
    )
    ax1.scatter(0, 0, 0)
    ax1.set_xlabel("Широта")
    ax1.set_ylabel("Долгота")
    ax1.set_zlabel("Высота в км")
    ax1.set_title("Траектория полёта")
    ax1.set_zlim3d([minZ, maxZ])
    ax1.set_ylim3d([minY, maxXY])
    ax1.set_xlim3d([minXY, maxX])
    ax1.view_init(15, 45)
    plt.show()

    return None


# # Press the green button in the gutter to run the script.
if __name__ == '__main__':
    plot3dTrajectory()



# # See PyCharm help at https://www.jetbrains.com/help/pycharm/
