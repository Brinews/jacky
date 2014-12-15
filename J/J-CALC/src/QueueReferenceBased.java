public class QueueReferenceBased 
implements QueueInterface {
  private Node lastNode;
  
  public QueueReferenceBased() {
    lastNode = null;   
  }  // end default constructor
  
  // queue operations:
  public boolean isEmpty() {
    return lastNode == null;
  }  // end isEmpty

  public void dequeueAll() {
    lastNode = null;
  }  // end dequeueAll

  public void enqueue(Object newItem) {
    Node newNode = new Node(newItem);

    // insert the new node
    if (isEmpty()) {
      // insertion into empty queue
      newNode.setNext(newNode);
    }
    else {
      // insertion into nonempty queue
      newNode.setNext(lastNode.getNext());
      lastNode.setNext(newNode);
    }  // end if

    lastNode = newNode;  // new node is at back
  }  // end enqueue

  public Object dequeue() throws QueueException {
    if (!isEmpty()) {
      // queue is not empty; remove front
      Node firstNode = lastNode.getNext();
      if (firstNode == lastNode) { // special case?
        lastNode = null;           // yes, one node in queue
      }
      else {
        lastNode.setNext(firstNode.getNext());
      }  // end if
      return firstNode.getItem();
    }
    else {
      throw new QueueException("QueueException on dequeue:"
                             + "queue empty");
    }  // end if
  }  // end dequeue

  public Object peek() throws QueueException {
    if (!isEmpty()) {  
      // queue is not empty; retrieve front
      Node firstNode = lastNode.getNext();
      return firstNode.getItem();
    }
    else {
      throw new QueueException("QueueException on peek:"
                             + "queue empty");
    }  // end if
  }  // end peek
   
} // end QueueReferenceBased