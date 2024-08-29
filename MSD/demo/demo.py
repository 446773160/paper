from loguru import logger
import pandas as pd
from typing import List
from itertools import dropwhile


class MSDTransform:
    def __init__(self):
        self.M = [[1, 0, -1], [0, 0, 0], [-1, 0, 1]]
        self.T = [[-1, -1, 0], [-1, 0, 1], [0, 1, 1]]
        self.W = [[0, 1, 0], [1, 0, -1], [0, -1, 0]]
        self.T_ = [[-1, 0, 0], [0, 0, 0], [0, 0, 1]]
        self.W_ = [[0, -1, 0], [-1, 0, 1], [0, 1, 0]]
        self.R = 0
        self.G = 0
        self.ARG_R = []
        self.ARG_G = []

    def decimal_to_binary_list(self, n):
        if n == 0:
            return [0]

        binary_list = []

        while n > 0:
            remainder = n % 2
            binary_list.insert(0, remainder)
            n = n // 2

        return binary_list

    def transform_msd(self):
        flag = False
        if self.R < 0:
            self.R = -self.R
            flag = True
        self.ARG_R = self.decimal_to_binary_list(self.R)
        if flag:
            self.ARG_R = [-i for i in self.ARG_R]
        flag = False
        if self.G < 0:
            self.G = -self.G
            flag = True
        self.ARG_G = self.decimal_to_binary_list(self.G)
        if flag:
            self.ARG_G = [-i for i in self.ARG_G]
        # if len(self.ARG_R) <= 33:
        #     self.ARG_R = [0]*(33-len(self.ARG_R))+self.ARG_R
        # if len(self.ARG_G) <= 33:
        #     self.ARG_G = [0]*(33-len(self.ARG_G))+self.ARG_G
        if len(self.ARG_R) > 33 or len(self.ARG_G) > 33:
            # logger.error("The number is too large to be converted to MSD")
            raise ValueError("The number is too large to be converted to MSD")

    def msd_to_decimal(self, msd):
        decimal = 0
        for i in range(len(msd)):
            decimal += msd[i] * (2 ** (len(msd) - i - 1))
        return decimal

    def resize(self, aux_R, aux_G):
        if len(aux_R) > len(aux_G):
            aux_G = [0] * (len(aux_R) - len(aux_G)) + aux_G
        if len(aux_R) < len(aux_G):
            aux_R = [0] * (len(aux_G) - len(aux_R)) + aux_R
        return aux_R, aux_G

    def transform_T(self, aux_R, aux_G):
        aux_R, aux_G = self.resize(aux_R, aux_G)
        result = []
        for i in range(len(aux_R)):
            result.append(self.T[aux_G[i]+1][aux_R[i]+1])
        return result

    def transform_W(self, aux_R, aux_G):
        aux_R, aux_G = self.resize(aux_R, aux_G)
        result = []
        for i in range(len(aux_R)):
            result.append(self.W[aux_G[i]+1][aux_R[i]+1])
        return result

    def transform_T_(self, aux_R, aux_G):
        aux_R, aux_G = self.resize(aux_R, aux_G)
        result = []
        for i in range(len(aux_R)):
            result.append(self.T_[aux_G[i]+1][aux_R[i]+1])
        return result

    def transform_W_(self, aux_R, aux_G):
        aux_R, aux_G = self.resize(aux_R, aux_G)
        result = []
        for i in range(len(aux_R)):
            result.append(self.W_[aux_G[i]+1][aux_R[i]+1])
        return result

    def transform_M(self, aux_R, b):
        result = []
        for i in range(len(aux_R)):
            result.append(self.M[b+1][aux_R[i]+1])
        return result


def compare_length(a, b):
    if len(a) > len(b):
        return b, a
    else:
        return a, b


class MSD:
    def __init__(self):
        self.tramsform = MSDTransform()
        self.result = []

    def msd_add(self, x, y):
        self.tramsform.R = x
        self.tramsform.G = y
        self.tramsform.transform_msd()
        u = self.tramsform.transform_T(
            self.tramsform.ARG_R, self.tramsform.ARG_G)
        w = self.tramsform.transform_W(
            self.tramsform.ARG_R, self.tramsform.ARG_G)
        u.append(0)
        u_ = self.tramsform.transform_T_(u, w)
        w_ = self.tramsform.transform_W_(u, w)
        u_.append(0)
        result = self.tramsform.transform_T(u_, w_)
        # logger.info(
        #     f"MSD of {x} + {y} = {result}, Decimal: {self.tramsform.msd_to_decimal(result)}")
        return result

    def bit_mul_transform(self, aux_R, aux_G):
        result = []
        aux_R, aux_G = compare_length(aux_R, aux_G)
        length = len(aux_R)-1
        for i in range(len(aux_R)):
            mid_result = self.tramsform.transform_M(aux_G, aux_R[i])
            mid_result = mid_result+[0]*length
            if len(mid_result) < 33:
                result.append((33-len(mid_result))*[0]+mid_result)
            if len(mid_result) > 33:
                result.append(mid_result[len(mid_result)-33:])
            length -= 1
        return result

    def msd_add_inner(self, x, y):
        self.tramsform.ARG_R = x
        self.tramsform.ARG_G = y
        u = self.tramsform.transform_T(
            self.tramsform.ARG_R, self.tramsform.ARG_G)
        w = self.tramsform.transform_W(
            self.tramsform.ARG_R, self.tramsform.ARG_G)
        u.append(0)
        u_ = self.tramsform.transform_T_(u, w)
        w_ = self.tramsform.transform_W_(u, w)
        u_.append(0)
        result = self.tramsform.transform_T(u_, w_)
        return result

    def msd_multiply(self, x, y):
        self.tramsform.R = x
        self.tramsform.G = y
        self.tramsform.transform_msd()
        self.result.extend([x, y])
        self.tramsform.ARG_R, self.tramsform.ARG_G
        ARG_R = []
        ARG_G = []
        ARG_R.extend(self.tramsform.ARG_R)
        ARG_G.extend(self.tramsform.ARG_G)
        self.result.append(ARG_R)
        self.result.append(ARG_G)
        mid_result = self.bit_mul_transform(
            self.tramsform.ARG_R, self.tramsform.ARG_G)
        temp = []
        temp.extend(mid_result)
        self.result.append(temp)
        for i in range(len(mid_result)-1):
            mid_result[i +
                       1] = self.msd_add_inner(mid_result[i+1], mid_result[i])
        if len(mid_result[-1]) <= 33:
            self.result.append((33-len(mid_result[-1]))*[0]+mid_result[-1])
        else:
            flag = False
            mid = []
            for i in mid_result[-1]:
                if i == 0 and not flag:
                    continue
                else:
                    flag = True
                    mid.append(i)
            if flag:
                if len(mid) <= 33:
                    self.result.append((33-len(mid))*[0]+mid)
                else:
                    # logger.error(
                    #     "The number is too large to be converted to MSD")
                    raise ValueError(
                        "The number is too large to be converted to MSD")
        # logger.info(
        #     f"MSD of {x} * {y} = {self.tramsform.msd_to_decimal(mid_result[-1])}")
        self.result.append(self.tramsform.msd_to_decimal(mid_result[-1]))
        if len(ARG_R) <= 33:
            ARG_R = [0]*(33-len(ARG_R))+ARG_R
        if len(ARG_G) <= 33:
            ARG_G = [0]*(33-len(ARG_G))+ARG_G
        self.result[2] = ARG_R
        self.result[3] = ARG_G
        return self.result

    def clear(self):
        self.result = []


def detail_data(mid_result, data):
    result = []
    print(mid_result)
    for i in mid_result:
        # logger.info(i)
        result.append(''.join([data[t] for t in i]))
    result = ', '.join(result)
    return result


class RowData:
    def __init__(self, x, y, msd_x, msd_y, mid_result, msd_result, result):
        self.x = x
        self.y = y
        self.msd_x = msd_x
        self.msd_y = msd_y
        self.mid_result = mid_result
        self.msd_result = msd_result
        self.result = result


if __name__ == '__main__':
    msd = MSD()
    multiplicands = [
        2, -4, 12, -23, -67, 123, -345,
        1234, -3456
    ]

    multipliers = [
        3, 5, 15, 34, -78, 234, 456,
        2345, 4567
    ]

    result = []
    for i in range(len(multiplicands)):
        result.append(msd.msd_multiply(multiplicands[i], multipliers[i]))
        msd.clear()
    # row_datas: List[RowData] = []
    # data = {-1: "u", 0: "0", 1: "1"}
    # for i in range(len(result)):

    #     msd_x = ''.join([data[t] for t in result[i][2]])
    #     msd_y = ''.join([data[t] for t in result[i][3]])
    #     mid_result = detail_data(result[i][4], data)
    #     msd_result = ''.join([data[t] for t in result[i][5]])

    #     row_datas.append(RowData(x=result[i][0], y=result[i][1], msd_x=msd_x, msd_y=msd_y,
    #                      mid_result=mid_result, msd_result=msd_result, result=str(result[i][6])))
    # # data = [{"x": row_data.x, "y": row_data.y, "msd_x": row_data.msd_x, "msd_y": row_data.msd_y, "mid_result": row_data.mid_result,
    # #          "msd_result": row_data.msd_result, "result": row_data.result}
    # #         for row_data in row_datas]
    # for row_data in row_datas:
    #     print(row_data.x, row_data.y, row_data.msd_x, row_data.msd_y)

        # df = pd.DataFrame(data)
        # df.to_excel('msd.xlsx', index=False)
    data = {-1: "u", 0: "0", 1: "1"}
    print("-----------------------------------------------------------------------")
    for i in range(len(result)):
        # print(len(result[i][2]))
        # msd_x = ''.join([data[t] for t in result[i][2]])
        # # print(len(result[i][3]))
        # msd_y = ''.join([data[t] for t in result[i][3]])
        # print(f"{msd_x},{msd_y}")
        for j in range(len(result[i][4])):
            print(
                f"{''.join([data[t] for t in result[i][4][j]])}", end=",")
        print()
        # print(f"第{i+1}次计算结果")
        # for j in range(len(result[i][2])):
        #     #     # print(list(dropwhile(lambda x: x == 0, result[i][4][j])), end=",")

        #     print(
        #         f"{''.join(map(str,result[i][4][j]))}", end=",")
        # print(f"int result_array{i+1}=", end="")
        # print("{", end="")
        # for j in result[i][5]:
        #     print(j, end=",")
        # print("}")
        #     print()
        # print(f"{''.join([data[t] for t in result[i][5]])}&{result[i][6]}")
        # print(f"{''.join([data[t] for t in result[i][4][0]])}")
