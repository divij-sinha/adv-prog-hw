# Homework 3

Implement a serial, two-dimensional version of the Barnes-Hut n-body algorithm as described in recorded lecture and course notes. Note that even the serial, 2D version can be tricky to implement efficiently, so I have provided some pseudocode and guidance in the recorded lectures in addition to a high-level description of the algorithm. There are three main parts and each requires care to get the details right. Step 1 is to build/add particles to the quadtree; Step 2 is to "summarize" the leaf nodes -- i.e., compute the center of mass at each parent; Step 3 is to scan the quadtree depth first left to right using the distance threshold to calculate the forces.

# Submission Requirements

Name - Divij Sinha  
cnet - divijs  
UchicagoID - 12364171

# Files

 Barnes-hut implementation in `barneshut.c`
 quadtree functions in the header file `quadtree.h`

## Declarations in header files 

The struct `Particle` has the following members -  
 - `x`, `y` - the coordinates of the point particle  
 - `vx`, `vy` - the velocities of the point particle  
 - `mass` - the mass of the particle, 1 in all cases  

The struct `QuadTreeNode` has the following members -  
 - `Particle part` - the current particle in this quad tree  
 - `int n_of_children` - the number of initialised children of this   node. Can take the following values -  
    - `0`: implies empty node  
    - `1`: implies leaf with a single part  
    - `4`: implies node with 4 children nodes  
 - `float bounds[4]` - the bounds in the up, down, left, right directions respectively  
 - `Particle center_of_mass` - the center of mass of the node  
 - `QuadTreeNode *(children[4]` - the 4 children nodes  

The following functions also exist in the header file -  

 `qtree_null_child`   
  arguments - `node`  
  returns - void  
  behaviour - sets all node children to `NULL`

 `qtree_init_node`   
  arguments - `node`, `up`, `down`, `left`, `right`  
  returns - void  
  behaviour - takes a `node`, inititalises all values to zero/NULL, sets bounds, and allocates memory to next gen of children

 `qtree_find_child_idx`   
  arguments - `node`, `part`  
  returns - `int idx`
  behaviour - takes a `node`, and a `part`, and returns which-th child of the `node`, the `part` would belong to, given its location and the `node`'s boundaries


 `qtree_add`   
  arguments - `node`, `part`  
  returns - void  
  behaviour - takes in a root node `node` and adds the particle `part` to the correct position. It moves through the quad tree finding the correct position


 `qtree_center_of_mass`  
  arguments - `node`   
  returns - `Particle center_of_mass`  
  behaviour - returns the center of mass of the given `node`. If the root node is supplied, it returns the center of mass of the whole system. As it sets the center of mass recursively for the whole tree, calling it on the node is enough to set the center of mass values for the whole tree


 `qtree_update`  
  arguments - `node`, `dt`, `BOUND`   
  returns - `float BOUND`   
  behaviour - iterates over the quad tree, and updates each value of `x`, `y` as defined by the `node` using `dt` and the new velocities in each particle

 `qtree_free`
  arguments - `node`  
  returns - `NULL`  
  behaviour - frees all allocated memory to a tree


 `qtree_calc_force_find_node`  
  arguments - `node cur_node`, `node root_node` 
  returns - void  
  behaviour - iterates over all the nodes until it reaches leaf nodes and then calculates the force per node

 `qtree_calc_force`  
  arguments - `cur_node`, `force_node`, `dt`, `thresh`  
  returns - `Force F` - the force acting on the `cur_node` from the `force_node`
  behaviour - checks if dist between `cur_node`, `force_node`, if dist < `thresh`, calls `qtree_calc_force` on all its children and returns its `Force`

  `qtree_print`  
  prints the whole tree


# Tests

Initially I ran the serial naive nbody code, and the bh implementation with no threshold, and the outputs were checked bytewise to be idential. For the approximations, I wrote a small python script to animate the behaviour of the particles to determine if it was consistent with naive behaviour

# Speed up metrics and reflections

The linear code runs with the following speeds for `nIters=200`

![linear_speeds](/static/SCR-20221111-42a.png)

Clearly, the time increases with `nParticles` as $n^2$

For the barneshut implementation, there are multiple parameters to take care of, namely - `dt` `threshold` `nParticles`

We alter all of them and see how the implementation behaves - 

Varying `dt` for `nParticles=3000`, `threshold=1.0`
![dt_change](/static/SCR-20221111-46e.png)

As we can see here, changing the `dt` has massive effects on the time taken. For smaller timesteps, less particles move out of threshold limit, increasing the calculations done


varying `threshold` for `nParticles=3000`, `dt=0.001`
![thresh_change](/static/SCR-20221111-475.png)

As we can see here, changing `thresholds` also varies the time taken, with lower thresholds being faster as more and more points fall outside the limits and fewer calculations need to be made


varying `nParticles` for `threshold=1.0`, `dt=0.001`
![n_change](/static/SCR-20221111-4iw.png)

As we can see here, this approximation becomes less and less effective as the number of particles decrease.


Therefore, this approximation is the fastest with larger time steps, lots of particles and low thresholds. However, we do need to be careful in implementing it as for large time steps, the values tend to "blow up", as particles shoot past each other and do not really slow down after that. Similarly, if we lower thresholds too much, the pattern of collapse is slightly changed with the points within the circle of the radius = threshold collapse quicker, and the remaining parts collapse slower as opposed almost all at the same time in the linear naive approach.

There is also the matter of the Softening variable, currently set to `1e-9` as initially given to us. This value seems to work well for the 3 dimensional case, however through trial and error, the value of `1e-6` gave us slightly better results with far fewer blowups. I am not entirely certain why this is, my guess would be something to do with the probability of 2 particles being really close together in 2 dimensions vs 3 dimensions in our initialised value space.



## visualisations of the quad tree grid

for 
 - `dt = 0.0001f`
 - `nParticles = 1000`
 - `threshold = 1.0f`
 - `nIters = 200`

we set the `SOFTENING` to `1e-6` to get prettier looking animations with no outliers

![anim](/static/anim_bh.mp4)
### initial grid 
![init](/static/init_grid.png)
### final grid 
![final](/static/final_grid.png)
(with blown up outliers, concentrated in the center)
