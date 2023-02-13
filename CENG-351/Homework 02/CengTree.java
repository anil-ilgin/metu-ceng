import java.util.ArrayList;


public class CengTree
{
    public CengTreeNode root;
    // Any extra attributes...

    public CengTree(Integer order)
    {
        root = new CengTreeNodeLeaf(null);
        CengTreeNode.order = order;
        root.level = 1;
        // TODO: Initialize the class

    }

    public void addVideo(CengVideo video)
    {
        CengTreeNodeLeaf node = (CengTreeNodeLeaf) root.addVideo(video);
        CengTreeNodeLeaf split_node;
        if (node.videoCount() > 2*root.getOrder() ) {
            split_node = (CengTreeNodeLeaf) node.split();
            copyUp(split_node);
        }
        // TODO: Insert Video to Tree
    }

    public ArrayList<CengTreeNode> searchVideo(Integer key)
    {
        // TODO: Search within whole Tree, return visited nodes.
        // Return null if not found.

        boolean loop = true;
        boolean check = true;
        boolean end_check = true;
        ArrayList<CengTreeNode> ret_arr = new ArrayList<>();
        CengTreeNode head = root;
        CengTreeNodeInternal node_1;
        int adress = 0;
        while(loop){
            if (head.getType() == CengNodeType.Internal) {
                end_check = true;
                node_1 = (CengTreeNodeInternal) head;
                for(int i=0; i < node_1.keyCount();i++) {
                    if(node_1.keyAtIndex(i) <= key) {
                    }
                    else {
                        ret_arr.add(head);
                        head = node_1.getAllChildren().get(i);
                        end_check = false;
                        break;
                    }
                }
                if(end_check) { ret_arr.add(head); head = node_1.getAllChildren().get(node_1.getAllChildren().size() - 1); }

            }
            else {

                CengTreeNodeLeaf node_2 = (CengTreeNodeLeaf) head;
                for(int i=0; i < node_2.videoCount(); i++) {
                    if(node_2.videoKeyAtIndex(i).equals(key)) {

                        ret_arr.add(head);
                        adress = i;
                        check = false;
                        break;
                    }
                }
                loop = false;
             }
        }
        if (check) {
            System.out.println("Could not found "+key);
            return null;
        }
        else {
            for(int i=0; i < ret_arr.size(); i++) {
                String tabs = "";
                for(int m=1; m < ret_arr.get(i).level; m++) {
                    tabs += "\t";
                }
                if(ret_arr.get(i).getType() == CengNodeType.Leaf) {
                    CengTreeNodeLeaf leaf = (CengTreeNodeLeaf) ret_arr.get(i);
                    CengVideo vid = leaf.videoAtIndex(adress);
                    System.out.println(tabs + "<record>" + vid.fullName() +"</record>");
                    break;
                }
                System.out.println(tabs + "<index>");
                for(int k=0; k < ret_arr.get(i).getSize(); k++) {
                    System.out.println(tabs + ret_arr.get(i).getPrint(k) );
                }
                System.out.println(tabs + "</index>");
            }
            return ret_arr;

        }
    }

    public void printTree()
    {
        root.print();
        // TODO: Print the whole tree to console
    }

    // Any extra functions...
    public void copyUp(CengTreeNode node ) {
        if(root.getType() == CengNodeType.Leaf) {
            CengTreeNodeInternal new_root = new CengTreeNodeInternal(null);
            CengTreeNode old_root = root;
            CengTreeNodeLeaf node_2 = (CengTreeNodeLeaf) node;
            new_root.getAllChildren().add(old_root);
            new_root.getAllChildren().add(node);
            new_root.getKeys().add(node_2.videoKeyAtIndex(0));
            root.level++;
            node.level++;
            root.setParent(new_root);
            node.setParent(new_root);
            root = new_root;

        }
        else {
            //Complete pushUp by splitting root or whatsoever
            CengTreeNodeInternal parent = (CengTreeNodeInternal) node.getParent();
            parent.addChildrenLeaf(node);
            if(parent.keyCount() > 2*root.getOrder()) {
                for(int i=0; i < parent.getAllChildren().size(); i++ ) parent.getAllChildren().get(i).level ++;
                pushUp(parent);
            }
        }

    }
    //Complete PushUp,
    public void pushUp (CengTreeNode node) {
        if(node.getParent() == null) {
            CengTreeNodeInternal new_root = new CengTreeNodeInternal(null);
            CengTreeNodeInternal split =  (CengTreeNodeInternal) root.split();
            Integer push_val = split.pop();
            new_root.getKeys().add(push_val);
            new_root.getAllChildren().add(root);
            new_root.getAllChildren().add(split);
            for(int i=0; i < split.getAllChildren().size(); i++) {
                split.getAllChildren().get(i).setParent(split);
            }
            root.setParent(new_root);
            root.level ++;
            split.setParent(new_root);
            split.level++;
            root = new_root;
        }
        else {
            CengTreeNodeInternal parent = (CengTreeNodeInternal) node.getParent();
            CengTreeNodeInternal split = (CengTreeNodeInternal) node.split();
            for(int i=0; i < split.getAllChildren().size(); i++) {
                split.getAllChildren().get(i).setParent(split);
            }
            boolean check = true;
            for(int i=0; i < parent.getAllChildren().size(); i++) {
                Integer split_key = split.getKeys().get(0);
                CengTreeNodeInternal parent_child = (CengTreeNodeInternal) parent.getAllChildren().get(i);
                int size_1 = parent_child.keyCount();
                Integer child_key = parent_child.keyAtIndex(size_1 - 1);
                if (split_key < child_key) {
                    parent.getAllChildren().add(i,split);
                    if (i < parent.keyCount() - 1) parent.getKeys().add(i,split_key);
                    else parent.getKeys().add(split_key);
                    check = false;
                    break;
                }
            }
            if (check) { parent.getAllChildren().add(split); parent.getKeys().add(split.getKeys().get(0)); }
            split.setParent(parent);
            split.pop();
            if (parent.keyCount() > 2*parent.getOrder()) {
                for(int i=0; i <parent.getAllChildren().size(); i++) parent.getAllChildren().get(i).level ++;
                pushUp(parent);
            }
        }
    }
}
