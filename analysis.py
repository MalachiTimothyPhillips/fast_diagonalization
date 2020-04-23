import numpy as np
import numpy.linalg as npla

u = np.loadtxt("u.dat")

if (len(u.shape) == 1):
    u = u.reshape(1, u.shape[0])

dim = 3
num_elements = u.shape[0]
N = int(np.sqrt(u.shape[1])) if (dim == 2) else int(np.cbrt(u.shape[1]))

Su = np.zeros((num_elements, N ** dim))

if (dim >= 1):
    S_x = np.loadtxt("S_x.dat").reshape(num_elements, N, N)
    L_x = np.loadtxt("L_x.dat").reshape(num_elements, N)

if (dim >= 2):
    S_y = np.loadtxt("S_y.dat").reshape(num_elements, N, N)
    L_y = np.loadtxt("L_y.dat").reshape(num_elements, N)

if (dim >= 3):
    S_z = np.loadtxt("S_z.dat").reshape(num_elements, N, N)
    L_z = np.loadtxt("L_z.dat").reshape(num_elements, N)

inv_L = np.loadtxt("inv_L.dat").reshape(num_elements, N ** dim)

I = np.eye(N)

# Apply FDM operator
for e in range(num_elements):
    if (dim == 2):
        S_xy = np.kron(S_y[e], S_x[e])
        inv_L_xy = np.kron(I, np.diag(1.0 / L_x[e])) + np.kron(np.diag(1.0 / L_y[e]), I)
        inv_A = np.dot(S_xy, np.dot(inv_L_xy, S_xy.T))
        Su[e] = np.dot(inv_A, u[e])

    else:
        S_xyz = np.kron(S_z[e], np.kron(S_y[e], S_x[e]))
        inv_L_xyz = np.kron(I, np.kron(I, np.diag(1.0 / L_x[e]))) + np.kron(I, np.kron(np.diag(1.0 / L_y[e]), I)) + np.kron(np.diag(1.0 / L_z[e]), np.kron(I, I))
        inv_A = np.dot(S_xyz, np.dot(inv_L_xyz, S_xyz.T))
        Su[e] = np.dot(inv_A, u[e])

        ################################################################
#        S_xyz_t = np.kron(S_z[e].T, np.kron(S_y[e].T, S_x[e].T))
#        S_xyz = np.kron(S_z[e], np.kron(S_y[e], S_x[e]))
#        print np.dot(S_xyz, np.dot(inv_L_xyz, np.dot(S_xyz_t, u[e])))
#        #print u[e]
#        print np.loadtxt("Su.dat")[e]
#        quit()
        ################################################################

print npla.norm(Su - np.loadtxt("Su.dat"))
