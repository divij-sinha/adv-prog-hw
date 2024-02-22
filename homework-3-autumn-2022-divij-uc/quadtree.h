typedef struct {
  float x, y/* , z */;        /* particle positions */
  float vx, vy/* , vz */;     /* particle momenta */
  float mass;
} Particle;

typedef struct {
  float Fx, Fy/* , Fz */;
} Force;

typedef struct quad_tree_node
{
  Particle *part;
  int n_of_children;
  float bounds[4]; /* up down left right */
  Particle center_of_mass;
  struct quad_tree_node *(children[4]); /* up-left up-right down-right down-left */

} QuadTreeNode;


void qtree_add(QuadTreeNode* node, Particle* part);
void qtree_print_node(QuadTreeNode* node, int depth);
void qtree_null_child(QuadTreeNode* node);
void qtree_init_node(QuadTreeNode* node, float up, float down, float left, float right);
int qtree_find_child_idx(QuadTreeNode* node, Particle* part);
Particle qtree_center_of_mass(QuadTreeNode *node);
float qtree_update(QuadTreeNode* node, float dt, float BOUND);
void qtree_free(QuadTreeNode* node);
Force qtree_calc_force(QuadTreeNode* cur_node, QuadTreeNode* force_node, float dt, float thresh);
void qtree_calc_force_find_node(QuadTreeNode* cur_node, QuadTreeNode* root_node, float dt, float thresh);
void qtree_print(QuadTreeNode *node, int depth);