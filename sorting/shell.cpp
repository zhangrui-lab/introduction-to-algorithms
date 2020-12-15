//
// Created by 张锐 on 2020/12/15.
//

/*
希尔排序（Shellsort）

算法首先将整个待排序向量A[]等效地视作一个二维矩阵B[][]。
若原一维向量为A[0, n)，则对于任一固定的矩阵宽度w，A与B中元素之间总有一一对应关系：  （对规模为n的输入序列进行等价类划分（模w同余））
    B[i][j] = A[i + jw]
    A[k] = B[k % w][k / w]
从秩的角度来看，矩阵B的各列依次对应于整数子集[0, n)关于宽度w的某一同余类。这也等效于从上到下、自左而右地将原向量A中的元素，依次填入矩阵B的各个单元。
为简化起见，以下不妨假设w整除n。如此，B中同属一列的元素自上而下依次对应于A中以w为间隔的n/w个元素。因此，矩阵的宽度w亦称作增量（increment）。

模w同余的等价类划分：
    当w越大时所含的等价类个数越多。w=1时，等价类只存在一个，即原输入序列。

算法框架
希尔排序的算法框架，可以扼要地描述如下：
    Shellsort(A, n):                             // 输入：规模为n的无序向量A； 输出：A的一个有序排列
        H = { w1 = 1, w2, w3, ..., wk, ... };    // 一个递增的增量序列
        wk = max{i | wi < n};                    // 即wk为增量序列H中小于n的最后一项
        for (t = k; t > 0; t--)                  // 缩小增量
            Bt[i]，i = 0, 1, ..., wt - 1;        // 将向量A规作以wt为宽度的二维矩阵B。对Bt的每一列分别排序;

每一步迭代中，都从事先设定的某个整数序列中取出一项，并以该项为宽度，将输入向量重排为对应宽度的二维矩阵，然后逐列分别排序。当然，各步迭代并不需要真地从物理上重排原向量。
事实上，借助以上一一对应关系，即可便捷地从逻辑上根据其在B[][]中的下标，访问统一保存于A[]中的元素。不过，为便于对算法的理解，以下我们不妨仍然假想地进行这一重排转换。
因为增量序列中的各项是逆向取出的，所以各步迭代中矩阵的宽度呈缩减的趋势，直至最终使用w1 = 1。矩阵每缩减一次并逐列排序一轮，向量整体的有序性就得以进一步改善。当增量缩
减至1时，矩阵退化为单独的一列，故最后一步迭代中的“逐列排序”等效于对整个向量执行一次排序。这种通过不断缩减矩阵宽度而逐渐逼近最终输出的策略，称作递减增量（diminishing increment）算法，这也是希尔排序的另一名称。

底层算法
最后一轮迭代等效于向量的整体排序，故无论此前各步如何迭代，最终必然输出有序向量，希尔排序的正确性毋庸置疑。然而反过来，我们却不禁有个疑问：既然如此，此前各步迭代中的逐列排序又有何必要？
为何不直接做最后一次排序呢？这涉及到底层排序算法的特性。能够有效支持希尔排序的底层排序算法，必须是输入敏感的，比如插入排序算法。尽管该算法在最坏情况下需要运行O(n^2)时间，但随着向量的有序性不断提高（即逆序对的不断减少），
运行时间将会锐减。具体地，当逆序元素的间距均不超过k时，插入排序仅需O(kn)的运行时间。正是得益于这一特性，各步迭代对向量有序性的改善效果，方能不断积累下来，后续各步迭代的计算成本也能得以降低，并最终将总体成本控制在足以令人满意的范围。

增量序列
希尔排序算法的主体框架已经固定，唯一可以调整的只是增量序列的设计与选用。事实上这一点也的确十分关键，不同的增量序列对插入排序以上特性的利用程度各异，算法的整体效率也相应地差异极大。
首先考查Shell本人在提出希尔算法之初所使用的序列：
    Hshell = { 1, 2, 4, 8, 16, 32, ..., 2^k , ... }
    我们将看到，若使用这一序列，希尔排序算法在最坏情况下的性能并不好。请注意，在Hshell中，首项之外的其余各项均为偶数。因此，在最后一步迭代之前，每组元素的秩依然保持最初的奇偶性不变。如果把它们分别比作井水与河水，则尽管井水与河水各自都在流动，但毕竟“井水不犯河水”。
    反观这一实例可见，导致最后一轮排序低效的直接原因在于，此前的各步迭代尽管可以改善两组元素各自内部的有序性，但对二者之间有序性的改善却于事无补。究其根源在于，序列Hshell中除首项外各项均被2整除。
    由此我们可以得到启发。为改进希尔排序的总体性能，首先必须尽可能减少不同增量值之间的公共因子。为此，一种彻底的方法就是保证它们之间两两互素。

h-有序与h-排序
    在向量S[0, n)中，若S[i] <= S[i + h]对任何0 <= i < n - h均成立，则称该向量 h-有序（h-ordered）。也就是说，其中相距h个单元的每对元素之间均有序。考查希尔排序中对应于任一增量h的迭代。
    如前所述，该步迭代需将原向量“折叠”成宽度为h的矩阵，并对各列分别排序。就效果而言，这等同于在原向量中以h为间隔排序，故这一过程称作h-排序（h-sorting）。不难看出，经h-排序之后的向量必然h-有序。
关于h-有序和h-排序，Knuth[3]给出了一个重要结论：
    已经g-有序的向量，再经h-排序之后，依然保持g-有序也就是说，此时该向量既是g-有序的，也是h-有序的，称作(g, h)-有序。

考查(g, h)-有序的任一向量S。借助有序性的传递律可知，相距 g + h的任何一对元素都必有序，故S必然(g + h)-有序。推而广之，对于任意非负整数m和n，相距mg + nh的任何一对元素都必有序，故S必然(mg + nh)-有序。

有序性的保持与加强
    根据以上Knuth所指出的性质，随着h不断递减，h-有序向量整体的有序性必然逐步改善。特别地，最终1-有序的向量，即是全局有序的向量。

在分别做过g-排序与h-排序之后，根据Knuth的结论可知该向量必已(g, h)-有序。由以上分析，对于g和h的任一线性组合mg + nh，该向量也应(mg + nh)-有序。因此反过来，逆序对的间距必不可能是g和h的组合。
而根据数论中的结论，只要g和h互素，逆序对的间距就绝不可能大于(g - 1)∙(h - 1)。由此可见，希尔排序过程中向量的有序性之所以会不断积累并改善，其原因可解释为，向量中每个元素所能参与构成的逆序对持续减少，
整个向量所含逆序对的总数也持续减少。与此同时，随着逆序对的减少，底层所采用的插入排序算法的实际执行时间，也将不断减少，从而提高希尔排序的整体效率。以下结合具体的增量序列，就此做出定量的估计。

(g, h)-有序与排序成本
设某向量S已属(g, h)-有序，且假设g和h的数值均处于O(d)数量级，以下考查对该向量做 d-排序所需的时间成本。据其定义，d-排序需将S等间距地划分为长度各为O(n / d)的d个子向量，并分别排序。
由以上分析，在(g, h)-有序的向量中，逆序对的间距不超过(g - 1)∙(h - 1)故就任何一个子向量的内部而言，逆序对的间距应不超过(g - 1)∙(h - 1) / d = O(d) 采用插入排序算法可在：
    O(d)∙(n / d) = O(n)的时间内，完成每一子向量的排序；于是，所有子向量的排序总体消耗的时间应不超过O(dn)。
*/

template<typename T>
void shellSort(T input[], size_t size) {
    for (int w = size/2; w > 0; w>>=1) {                    // 递减增量（不断缩减矩阵的宽度，直至剩下一列）
        for (int i = 0, j=i-w; i < size; i+=w,j=i-w) {      // 以i + xw将原输入序列映射到矩阵中； 底层采用插入排序算法
            T x = input[i];
            while (j > -1 && input[j] > x) {
                input[j+w] = input[j];
                j-=w;
            }
            input[j+w] = x;
        }
    }
}
