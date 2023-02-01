import numpy as np
from numpy import sin, cos
import os


def RX(t) -> np.ndarray:
    return np.array([
        [      1,       0,       0],
        [      0,  cos(t), -sin(t)],
        [      0,  sin(t),  cos(t)] 
    ])

def RY(t) -> np.ndarray:
    return np.array([
        [  cos(t),      0, sin(t)],
        [       0,      1,      0],
        [ -sin(t),      0, cos(t)] 
    ])

def RZ(t) -> np.ndarray:
    return np.array([
        [  cos(t), -sin(t),     0],
        [  sin(t),  cos(t),     0],
        [       0,       0,     1] 
    ])


def render_frame(A, B) -> None:

    screen_width, screen_height = 20, 40
    theta_spacing, phi_spacing = 0.07, 0.02

    R1, R2 = 1, 2
    K2 = 5
    K1 = screen_width*K2*3/(8*(R1+R2))

    RAY = np.array([0, 1, -1])

    output = np.array([[' ']*(screen_height+1)]*(screen_width+1))
    zbuffer = np.array([[0]*(screen_height+1)]*(screen_width+1))

    for phi in np.arange(0, 2*np.pi, phi_spacing):
        for theta in np.arange(0, 2*np.pi, theta_spacing):

            RotMat = RY(phi)@RX(A)@RZ(B)

            # ドーナツの断面
            donuts2d = np.array([R2+R1*cos(theta), R1*sin(theta), 0])

            # 断面を回転させて立体のドーナツに
            donuts3d = donuts2d@RotMat
            x, y, z = donuts3d
            screen_x , screen_y = int(screen_width/2 + (K1*x)/(K2+z)), int(screen_height/2 + (K1*y)/(K2+z))

            # 光の計算
            N = np.array([cos(theta), sin(theta), 0])@RotMat
            L = np.sum(N*RAY)

            # 光が届く
            if L > 0:
                # 今までで近い場合
                if z < zbuffer[screen_x][screen_y]:

                    # z-bufferとscreenの更新
                    output[screen_x][screen_y] = ".,-~:;=!*#$@"[int(min(L*8, 11))]
                    zbuffer[screen_x][screen_y] = z
            
    os.system('clear' if os.name == 'posix' else 'cls')
    for iy in output:
        for ix in iy:
            print(ix, end='')
        print()




if __name__ == '__main__':
    A, B = 0, 0
    while True:
        render_frame(A, B)
        A += 0.2
        B += 0.1