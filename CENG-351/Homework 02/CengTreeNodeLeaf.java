import java.util.ArrayList;
import java.util.List;

public class CengTreeNodeLeaf extends CengTreeNode
{
    private ArrayList<CengVideo> videos;
    // TODO: Any extra attributes

    public CengTreeNodeLeaf(CengTreeNode parent)
    {
        super(parent);
        videos = new ArrayList<>();
        type = CengNodeType.Leaf;
        // TODO: Extra initializations
    }

    // GUI Methods - Do not modify
    public int videoCount()
    {
        return videos.size();
    }
    public Integer videoKeyAtIndex(Integer index)
    {
        if(index >= this.videoCount()) {
            return -1;
        } else {
            CengVideo video = this.videos.get(index);

            return video.getKey();
        }
    }


    // Extra Functions
    public CengTreeNode addVideo(CengVideo video) {
        int size = videos.size();
        boolean check = true;
        for(int i=0; i < size; i++) {
            if(video.getKey() < videos.get(i).getKey()) { videos.add(i,video); check = false; break; }
        }
        if (check) { videos.add(video); }
        return this;
    }
    public CengTreeNode split() {
        CengTreeNodeLeaf new_node = new CengTreeNodeLeaf(this.getParent());
        int index = videos.size() / 2;
        new_node.videos.addAll(videos.subList(index,videos.size()));
        int size = videos.size();
        for(int i=index; i < size; i++) {
            videos.remove(index);
        }
        return new_node;
    }
    public CengVideo videoAtIndex(int i) {
        return videos.get(i);
    }
    public Integer getSize() { return videos.size(); }
    public Integer getPrint(int index) { return videos.get(index).getKey(); }
    public void print() {
        String tabs = "";
        for(int i=1; i < level; i++) {
            tabs += "\t";
        }
        System.out.println(tabs + "<data>");
        for(int i=0; i < videoCount(); i++) {
            CengVideo vid = videos.get(i);
            System.out.println(tabs + "<record>" + vid.fullName() +"</record>" );
        }
        System.out.println(tabs + "</data>");
    }

}