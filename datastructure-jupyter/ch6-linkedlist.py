class Node:
  def __init__(self, data, next=None):
    self.data = data
    self.next = next

# node1 과 node2 연결하기
node1 = Node(1)
node2 = Node(2)
node1.next = node2
head = node1