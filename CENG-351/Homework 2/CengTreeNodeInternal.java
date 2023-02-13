import java.util.ArrayList;

public class CengTreeNodeInternal extends CengTreeNode
{
    private ArrayList<Integer> keys;
    private ArrayList<CengTreeNode> children;

    public CengTreeNodeInternal(CengTreeNode parent)
    {
        super(parent);
        keys = new ArrayList<Integer>();
        children = new ArrayList<CengTreeNode>();
        type = CengNodeType.Internal;
        // TODO: Extra initializations, if necessary.
    }

    // GUI Methods - Do not modify
    public ArrayList<CengTreeNode> getAllChildren()
    {
        return this.children;
    }
    public Integer keyCount()
    {
        return this.keys.size();
    }
    public Integer keyAtIndex(Integer index)
    {
        if(index >= this.keyCount() || index < 0)
        {
            return -1;
        }
        else
        {
            return this.keys.get(index);
        }
    }

    // Extra Functions
    public ArrayList<Integer> getKeys() { return this.keys; }
    public CengTreeNode addVideo(CengVideo video) {
        int size = keys.size();
        CengTreeNode node;
        int i = 0;
        CengTreeNode ret_val = null;
        boolean check = true;
        if (size == 0) {
            node = new CengTreeNodeLeaf(this);
            node.addVideo(video);
            keys.add(video.getKey());
            children.add(node);
            return node;
        }
        for(; i < size ; i++ ) {
            if (video.getKey() < keys.get(i) ) {
                node = children.get(i);
                 ret_val = node.addVideo(video);
                check = false;
                break;
            }
        }
        if (check) {
            node = children.get(i) ;
            ret_val = node.addVideo(video);
        }
        return ret_val;
    }
    public CengTreeNode split() {
        CengTreeNodeInternal new_node = new CengTreeNodeInternal(this.getParent());
        int index = this.getAllChildren().size() / 2;
        int index_key = this.keyCount() / 2;
        new_node.children.addAll(children.subList(index, children.size()));
        new_node.keys.addAll(keys.subList(index_key,keys.size()));
        keys.subList(index_key,keys.size()).clear();
        children.subList(index, children.size()).clear();

        return new_node;
    }

    public void addChildrenLeaf(CengTreeNode node) {
        CengTreeNodeLeaf node_2 = (CengTreeNodeLeaf) node;
        int size = keys.size();
        boolean check = true;
        for(int i=0; i < size; i++) {
            if(node_2.videoKeyAtIndex(0) < keys.get(i) ) {
                children.add(i+1,node);
                keys.add(i,node_2.videoKeyAtIndex(0));
                check = false;
                break;
            }
        }
        if (check) {
            children.add(node);
            keys.add(node_2.videoKeyAtIndex(0));
        }

    }


    public Integer pop () {
        Integer key = keys.get(0);
        keys.remove(0);
        return key;
    }
    public Integer getSize() {return keys.size();}
    public Integer getPrint(int index) {return keys.get(index); }
    public void print() {
        String tabs = "";
        for(int i=1; i < level; i++ ) {
            tabs += "\t";
        }
        System.out.println(tabs + "<index>");
        for(int i=0; i < keys.size(); i++) {
            System.out.println(tabs + keys.get(i));
        }
        System.out.println(tabs + "</index>");
        for(int i=0; i < children.size(); i++) {
            children.get(i).print();
        }
    }

}