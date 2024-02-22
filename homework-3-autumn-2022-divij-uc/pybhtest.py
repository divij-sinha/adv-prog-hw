# %%
import numpy as np

class Particle():
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.vx = 0
        self.vy = 0

    def __str__(self):
        return " ".join(["Point:",str(self.x),str(self.y)])

class QTreeNode:
    def __init__(self,bd,bu,br,bl):
        self.children = 0 ##
        self.bd, self.bu, self.br, self.bl = bd,bu,br,bl ##
        self.cul, self.cur, self.cdr, self.cdl = 0,0,0,0 ##
        self.center_of_mass = 0 ##
        self.part = 0 ##

    def __str__(self):
        return " ".join(["Up-down: ", str(self.bu), str(self.bd), "Left-right:", str(self.bl), str(self.br)])


def which_node(node, part):
    if part.x > (node.br+node.bl/2):
        if part.y > (node.bd+node.bu)/2:
            return node.cur
        else:
            return node.cdr
    else:
        if part.y > (node.bd+node.bu)/2:
            return node.cul
        else:
            return node.cdl


depth = 0

def qtree_add(node, part):
    global depth
    depth+=1
    if node.children == 0:
        node.part = part
        node.children = 1
        return node
    elif node.children == 1:
        node.cul = QTreeNode((node.bd+node.bu)/2,node.bu,           (node.br+node.bl)/2,            node.bl)
        node.cur = QTreeNode((node.bd+node.bu)/2,node.bu,           node.br,            (node.br+node.bl)/2)
        node.cdr = QTreeNode(node.bd,           (node.bd+node.bu)/2,node.br,            (node.br+node.bl)/2)
        node.cdl = QTreeNode(node.bd,           (node.bd+node.bu)/2,(node.br+node.bl)/2,            node.bl)
        node.children = 4

        if node.part.x > (node.br+node.bl)/2:
            if node.part.y < (node.bd+node.bu)/2:
                node.cur = qtree_add(node.cur, node.part)
            else:
                node.cdr = qtree_add(node.cdr, node.part)
        else:
            if node.part.y < (node.bd+node.bu)/2:
                node.cul = qtree_add(node.cul, node.part)
            else:
                node.cdl = qtree_add(node.cdl, node.part)
        if part.x > (node.br+node.bl)/2:
            if part.y < (node.bd+node.bu)/2:
                node.cur = qtree_add(node.cur, part)
            else:
                node.cdr = qtree_add(node.cdr, part)
        else:
            if part.y < (node.bd+node.bu)/2:
                node.cul = qtree_add(node.cul, part)
            else:
                node.cdl = qtree_add(node.cdl, part)
        return node
    else:
        if part.x > (node.br+node.bl)/2:
            if part.y < (node.bd+node.bu)/2:
                node.cur = qtree_add(node.cur, part)
            else:
                node.cdr = qtree_add(node.cdr, part)
        else:
            if part.y < (node.bd+node.bu)/2:
                node.cul = qtree_add(node.cul, part)
            else:
                node.cdl = qtree_add(node.cdl, part)
        return node


def calc_com(node):
    if node.children == 1:
        node.center_of_mass = node.part
        return node.center_of_mass, 1
    elif node.children == 0:
        return node.center_of_mass, 0
    else:
        mass_pt_x = 0
        mass_pt_y = 0
        local_n = 0
        for n in [node.cul, node.cur, node.cdr, node.cdl]:
            n_com, n_n = calc_com(n)
            if n_n:
                local_n+=n_n
                mass_pt_x+= n_com.x
                mass_pt_y+= n_com.y

        node.center_of_mass = Particle(mass_pt_x/local_n, mass_pt_y/local_n)
        return node.center_of_mass, local_n



#%%

N = 2
plist = []
root = QTreeNode(1,0,1,0)

x_=0
y_=0

for i in range(N):
    part = Particle(*np.random.rand(2))
    x_+=part.x
    y_+=part.y
    plist.append(part)
    root = qtree_add(root, part)

print("returned!")

print(x_/N, y_/N)

calc_com(root)

print(root.center_of_mass)


# %%
