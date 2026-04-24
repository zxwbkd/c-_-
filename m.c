#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

struct Bookinformation
{
    char bookName[40];   // 书名
    char author[20];     // 作者
    char bookId[20];     // 书号
    float price;         // 价格 
    int count;           // 库存
    int totalCount;      // 总数
};

//用链表处理数据
struct Node
{
    struct Bookinformation data;
    struct Node *next;
};

struct Node* list = NULL;

//创建表头：表头是一个结构体变量
struct Node* createHead()       //createHead函数返回一个指向Node结构体的指针，表示链表的头节点
{
    //动态内存申请
    struct Node* headNode = (struct Node*)malloc(sizeof(struct Node));      //定义了一个Node类型的指针变量headNode（表头），指向在“堆”中申请的内存首地址
    //初始化指针域  
    headNode->next = NULL;
    //返回表头的指针
    return headNode;
}

//创建节点：创建一个节点，为插入数据做准备
struct Node* createNode(struct Bookinformation data)
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));      
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

//插入：使用表头插入法，把新的节点插入在headNode和headNode->next之间
//参数：headNode是表头，data是要插入的数据
void insertNodeByHead(struct Node* headNode, struct Bookinformation data)
{
    struct Node* newNode = createNode(data);     //创建一个新的节点，并将要插入的数据传递给createNode函数
    newNode->next = headNode->next;              //第一步：newNode->next = headNode->next  
    headNode->next = newNode;                    //第二步：headNode->next = newNode;
}

//删除：找到要删除的节点和它的前一个节点，然后修改前一个节点的next指针指向被删除的节点的下一个节点，最后释放要删除节点的内存
//重点是查找被删除的节点和它的前一个节点
//参数：headNode是表头，postdata是要删除的节点数据
//第一步：posleftNode->next = posNode->next;
//第二步：free(posNode);
void deleteNodeByBookName(struct Node* headNode,  char  *bookName)
{
    //让posleftNode从表头开始，一直向后移动，直到posNode等于要删除的节点数据或者posNode为NULL
    struct Node* posleftNode = headNode;
    struct Node* posNode = headNode->next;
    while(posNode != NULL && strcmp(posNode->data.bookName, bookName) )
    {
        posleftNode = posNode;
        posNode = posNode->next;
    }
    //讨论查询结果以及实际删除的过程
    if(posNode == NULL)
    {
        return;     //要删除的节点为空，直接返回
    }
    else 
    {   
        printf("删除成功\n");
        posleftNode->next = posNode->next;
        free(posNode);
        posNode = NULL;
    }
}

struct Node* searchBook (struct Node*  headNode,char* key ,int type)   //根据书名、作者、书号等信息查找书籍
{
    struct Node* posNode = headNode->next;
    while(posNode != NULL)
    {
        int isMatch = 0;
        switch (type)
        {
            case 1:     //按照书名查找
                if (strcmp(posNode->data.bookName, key) == 0) isMatch = 1;
                break;
            case 2:     //按照作者查找
                if (strcmp(posNode->data.author, key) == 0) isMatch = 1;
                break;
            case 3:     //按照书号查找
                if (strcmp(posNode->data.bookId, key) == 0) isMatch = 1;
                break;
        }
        if (isMatch) return posNode;   //如果找到匹配的节点，直接返回该节点的指针
        posNode = posNode->next;
    }
    return NULL;   //如果遍历完整个链表都没有找到匹配的节点，返回NULL
}


//打印链表
void printList(struct Node* headNode)   //遍历链表并打印每个有效节点的数据
{
    struct Node* pMove = headNode->next;     //定义一个指针变量pMove，指向链表的第一个节点（即headNode的下一个节点）
    printf("%-25s %-20s %-12s %-10s %-8s\n", "书名", "作者", "书号", "价格", "库存");
    while(pMove != NULL)
    {
        printf("%-25s %-20s %-12s %-10.2f %-8d\n", pMove->data.bookName, pMove->data.author, pMove->data.bookId, pMove->data.price, pMove->data.count);
        pMove = pMove->next;
    }
}

//菜单
void makeMenu()
{
    printf("--------------------------------------------------\n");
    printf("    xxoo图书管理系统\n");
    printf("\t0.退出系统\n");
    printf("\t1.登记书籍\n");
    printf("\t2.浏览书籍\n");
    printf("\t3.借阅书籍\n");
    printf("\t4.归还书籍\n");
    printf("\t5.删除书籍\n");
    printf("\t6.查找书籍\n");
    printf("--------------------------------------------------\n");
    printf("请输入(0~6):");
}
//文件存储操作
//参数：fileName是文件名，headNode是链表的头指针
void saveInfoToFile(const char *fileName, struct Node* headNode)
{
    FILE *fp = fopen(fileName, "w");    //以写入模式打开文件
if (fp == NULL)
    {
        printf("文件打开失败！\n");
        return;
    }
    struct Node* pMove = headNode->next;    
    while(pMove != NULL)              ////pMove从链表的第一个节点开始，依次遍历链表，将每个节点的数据写入文件中
    {
        
        fprintf(fp, "%s|%s|%s|%.2f|%d|%d\n",
            pMove->data.bookName,
            pMove->data.author,
            pMove->data.bookId,
            pMove->data.price,
            pMove->data.count,
            pMove->data.totalCount);

        pMove = pMove->next;
    }
    fclose(fp);   //关闭文件
}

//文件读取操作
void readInfoFromFile(const char *fileName, struct Node* headNode)
{
    FILE *fp = fopen(fileName, "r");    //以读取模式打开文件
    if (fp == NULL)
    {
        //第一次打开时文件肯定不存在
        fp = fopen(fileName, "w+");     //如果文件不存在，则创建一个新的文件
         return;    
     }
    struct Bookinformation tempData;
    while (fscanf(fp, "%[^|]|%[^|]|%[^|]|%f|%d|%d",
        tempData.bookName,
        tempData.author,
        tempData.bookId,
        &tempData.price,
        &tempData.count,
        &tempData.totalCount) == 6)
    {
        insertNodeByHead(headNode, tempData);
    }
    fclose(fp);   //关闭文件
}

void bubbleSortList(struct Node* headNode , int type)
{
    for (struct Node*p = headNode->next; p != NULL; p = p->next)
    {
        for (struct Node*q = p->next; q != NULL; q = q->next)
        {   
            int shouldSwap = 0;
            switch (type)
            {
                case 1:     //按照价格从小到大排序
                    if (p->data.price > q->data.price) shouldSwap = 1;
                    break;
                case 2:     //按照数量从小到大排序
                    if (p->data.count > q->data.count) shouldSwap = 1;
                    break;
                case 3:     //按照书号排序
                    if (strcmp(p->data.bookId, q->data.bookId) > 0) shouldSwap = 1;
                    break;
                case 4:     //按照书名排序
                    shouldSwap = strcmp(p->data.bookName, q->data.bookName) > 0;
                    break;
                case 5:     //按照作者排序
                    shouldSwap = strcmp(p->data.author, q->data.author) > 0;
                    break;

            }
            if (shouldSwap)
            {
                //交换数据
                struct Bookinformation tempData = p->data;
                p->data = q->data;
                q->data = tempData;
            }
        }
    }
    printList(headNode);
}

//交互
void  KeyDown()
{
    int userKey = 0;
    struct Bookinformation tempBook;           //定义一个临时变量存储书籍信息
    struct Node*  result =NULL;              //定义一个指针变量存储查找结果
    char key[40];                           //定义一个字符数组存储用户输入的关键字

    scanf("%d", &userKey);
    switch (userKey)
    {
        
    case 0:
        printf("【 退出 】\n");
        printf("退出成功\n");
        system("pause");
        exit(0);                    //关闭程序
        break;

    case 1:
        printf("【 登记 】\n");

        getchar(); // 读取并丢弃输入缓冲区中的换行符，确保后续的 fgets 能正确读取用户输入

        printf("请输入书名: ");
        fgets(tempBook.bookName, sizeof(tempBook.bookName), stdin);
        tempBook.bookName[strcspn(tempBook.bookName, "\n")] = '\0';         // 去掉字符串末尾的换行符

        printf("请输入作者: ");
        fgets(tempBook.author, sizeof(tempBook.author), stdin);
        tempBook.author[strcspn(tempBook.author, "\n")] = '\0';

        printf("请输入书号: ");
        scanf("%s", tempBook.bookId);   //  书号一般不含空格，可以用 scanf

        // 检查书号是否已存在
        {
        struct Node* p = list->next;
        int exist = 0;

        while (p != NULL)
        {
            if (strcmp(p->data.bookId, tempBook.bookId) == 0)
            {
                exist = 1;
                break;
            }
            p = p->next;
        }

        if (exist)
        {
            printf(" 录入失败：书号已存在，无法重复登记！\n");
            break;   // 直接结束本次录入
        }
        }

        printf("请输入价格和总数: ");
        scanf("%f %d", &tempBook.price, &tempBook.totalCount);

        tempBook.count = tempBook.totalCount;
        insertNodeByHead(list, tempBook);           //将登记的书籍信息插入到链表中
        saveInfoToFile("book.txt", list);       //将链表保存到文件中
        break;
    case 2:
        printf("【 浏览 】\n");
        printf("请选择书籍排序方式：1.价格 2.数量 3.书号 4.书名 5.作者\n");
        int sortType;     //定义一个变量sortType来存储用户选择的排序方式
        scanf("%d", &sortType);
            if (sortType >= 1 && sortType <= 5)
            {
                bubbleSortList(list, sortType);     //调用bubbleSortList函数对链表进行排序，并将排序结果打印出来
            }
            else 
            {
            printf("输入错误，将按照默认顺序浏览\n");
            printList(list);
            }

        break;
    case 3:
        printf("【 借阅 】\n");
        printf("请选择借阅方式: 1.书名 2.作者 3.书号\n");
        int borrowType;     //定义一个变量borrowType来存储用户选择的借阅方式
        scanf("%d", &borrowType);

        printf("请输入要借阅的书籍关键词：");
        scanf("%s", key);   //从用户输入中读取要借阅的书籍关键词，并将其存储在key数组中

        result = searchBook(list, key, borrowType);     //调用searchBook函数查找指定书籍，并将结果存储在result变量中
        if (result == NULL)
        {
            printf("相关书籍无法借阅\n");
        }
        else 
        {
            if(result->data.count > 0 )
            {
                result->data.count--;
                printf("借阅成功!当前库存: %d\n", result->data.count);
                saveInfoToFile("book.txt", list);     //修改完链表后，用saveInfoToFile函数将链表中的数据保存到文件中
            }
            else
            {
                printf("该书籍库存不足,借阅失败\n");
            }
        }
        break;
    case 4:
        printf("【 归还 】\n");
        printf("请选择归还方式: 1.书名 2.作者 3.书号\n:");
        int returnType;     //定义一个变量returnType来存储用户选择的归还方式
        scanf("%d", &returnType);

        printf("请输入要归还的书籍关键词：");
        scanf("%s", key);   //从用户输入中读取要归还的书籍关键词，并将其存储在key数组中

        result = searchBook(list, key, returnType);     //调用searchBook函数查找指定书籍，并将结果存储在result变量中
        if (result == NULL)
        {
            printf("该书来源异常(未找到登记信息)\n");
        }
        else 
        {   
            if(result->data.count < result->data.totalCount)
            {
                result->data.count++;
                printf("归还成功！当前库存: %d\n", result->data.count);
                saveInfoToFile("book.txt", list);     //修改完链表后，用saveInfoToFile函数将链表中的数据保存到文件中
            }
            else
            {
                printf("归还失败！该书籍已全部归还\n");
            }
        }
        break;
    case 5:
        printf("【 删除 】\n");
        printf("请输入要删除的书名：");
        scanf("%s", tempBook.bookName);   //从用户输入中读取要删除的书籍名称，并将其存储在tempBook变量的bookName字段中
        
        deleteNodeByBookName(list, tempBook.bookName);     //调用deleteNodeByBookName函数删除指定书籍
        saveInfoToFile("book.txt", list);     //删除后保存到文件
        break;
    case 6:
        printf("【 查找 】\n");
        printf("请选择查找方式: 1.书名 2.作者 3.书号\n");

        int searchType;     //定义一个变量searchType来存储用户选择的查找方式
        scanf("%d", &searchType);

        printf("请输入要查找书籍的关键字：");
        scanf("%s", key);   //从用户输入中读取要查找的关键字，并将其存储在key数组中
        result = searchBook(list, key, searchType);     //调用searchBook函数查找指定书籍，并将结果存储在result变量中
        if (result == NULL)
        {
            printf("未找到相关信息！\n");
        }
        else
        {
            printf("%-20s %-15s %-10s %-10s %-8s\n", "书名", "作者", "书号", "价格", "库存");
            printf("%-20s %-15s %-10s %-10.2f %-8d\n", result->data.bookName, result->data.author, result->data.bookId, result->data.price, result->data.count);
        }
        break;
    default:
        printf("【 输入错误 】\n");
        break;
    }

}

int main()
{   
    list=createHead();
    readInfoFromFile("book.txt", list);

    while (1)
    {
        makeMenu();
        KeyDown();
        system("pause");
        system("cls");
    }
    system("pause");
    return 0;
}