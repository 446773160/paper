import heapq
import matplotlib
matplotlib.use('TkAgg')  # 使用TkAgg后端，也可以尝试其他可用的后端
import matplotlib.pyplot as plt


# 定义节点类
class Node:
    def __init__(self, x, y, cost, heuristic, parent=None):
        self.x = x
        self.y = y
        self.cost = cost
        self.heuristic = heuristic
        self.parent = parent

    def __lt__(self, other):
        return (self.cost + self.heuristic) < (other.cost + other.heuristic)

# A*算法函数
def astar_search(parking_lot, start, goal):
    directions = [(-1, 0), (0, -1), (1, 0), (0, 1)]  # 上下左右四个方向

    rows, cols = len(parking_lot), len(parking_lot[0])

    def heuristic(node):
        return abs(node.x - goal[0]) + abs(node.y - goal[1])  # 曼哈顿距离作为启发式函数

    # 初始化起始节点
    start_node = Node(start[0], start[1], 0, heuristic(Node(start[0], start[1], 0, 0)))
    frontier = [start_node]
    visited = set()

    while frontier:
        current_node = heapq.heappop(frontier)

        if (current_node.x, current_node.y) == goal:
            path = []
            while current_node:
                path.append((current_node.x, current_node.y))
                current_node = current_node.parent
            return path[::-1]  # 返回路径（从终点到起点）

        visited.add((current_node.x, current_node.y))

        for dx, dy in directions:
            new_x, new_y = current_node.x + dx, current_node.y + dy
            if 0 <= new_x < rows and 0 <= new_y < cols and parking_lot[new_x][new_y] != 1 and (new_x, new_y) not in visited:
                new_cost = current_node.cost + 1  # 假设每一步代价为1
                new_node = Node(new_x, new_y, new_cost, heuristic(Node(new_x, new_y, 0, 0)), parent=current_node)
                heapq.heappush(frontier, new_node)
                visited.add((new_x, new_y))

    return None  # 如果找不到路径，返回None

# 给定停车场地图
parking_lot = [
    [1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1],  # 0
    [2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2],  # 1
    [1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1],  # 2
    [2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2],  # 3
    [1, 0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0, 1],  # 4
    [2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2],  # 5
    [1, 0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0, 1],  # 6
    [2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2],  # 7
    [1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1],  # 8
    [2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2],  # 9
    [1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1],  # 10
    [2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1],  # 11
    [1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1],  # 12
    [2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 2],  # 13
    [1, 0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0, 1],  # 14
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2],  # 15
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1],  # 16
    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2],  # 17
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],  # 18
]
def visualize_path(parking_lot, path):
    obstacle_color = 'black'  # 障碍物颜色为黑色正方形
    target_color = 'green'    # 目标地址颜色为绿色正方形
    path_color = 'red'        # 路径颜色为红色圆点

    for i in range(len(parking_lot)):
        for j in range(len(parking_lot[0])):
            if parking_lot[i][j] == 1:
                plt.plot(j, i, 's', color=obstacle_color)  # 障碍物用黑色正方形表示
            elif parking_lot[i][j] == 2:
                plt.plot(j, i, 's', color=target_color)    # 目标地址用绿色正方形表示

    for x, y in path:
        plt.plot(y, x, 'o', color=path_color)  # 路径用红色圆点表示
        plt.pause(0.1)

    plt.xlim(-1, len(parking_lot[0]))
    plt.ylim(-1, len(parking_lot))
    plt.gca().invert_yaxis()
    plt.show()



if __name__ == '__main__':
    start_point = None
    end_points = []

    # 查找起点和终点
    for i in range(len(parking_lot)):
        for j in range(len(parking_lot[0])):
            if parking_lot[i][j] == 2:
                end_points.append((i, j))
    start_point = (17, 0)
    for end_point in end_points:
        result = astar_search(parking_lot, start_point, end_point)
        if result:
            print("找到最短路径：", result)
            visualize_path(parking_lot, result)
            break
        else:
            print("无法找到路径")