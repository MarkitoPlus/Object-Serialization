特别声明/鸣谢：
  1)本项目中有关自定义结构体模块，本人主要阅读参考了csdn博客 <<C++ 实现序列化>> 作者 <leapmotion> (网址如下:https://blog.csdn.net/leapmotion/article/details/83687517#:~:text=%E6%9C%AC%E6%96%87%E7%9A%84%E5%BA%8F%E5%88%97%E5%8C%96%EF%BC%9A%E7%AE%80,%EF%BC%8C%E8%BF%99%E6%A0%B7%E6%96%B9%E4%BE%BF%E4%BC%A0%E8%BE%93%E7%AD%89)
  使用宏定义的实现思想。
  2)本项目中有关二进制序列化方法的实现思想受启发于数据库系统中编写的minisql序列化与反序列化框架，在项目中相关代码板块仅参考了文章中的思路。
  3）以上为本人有关项目中相关代码板块的思路来源的声明，保证代码细节由本人亲自实现。  
  
6/6 
熟悉项目要求，基于VScode搭建工程环境。初步编写BinarySerialization.h databuffer.h macro.h type.h头文件，初步建立项目框架.

6/7
全部完成BinarySerialization.h databuffer.h macro.h type.h搭建,同时明确了工程实现上的主要优势与缺陷：
  优势:
    1. 封装性：几乎所有的数据结构仅仅需要与二进制序列化执行器交互即可完成所有操作，即使是用户自定义结构体也仅仅需要继承一个接口（纯虚函数类）并添加一段宏 即可。
    2. 层次性：整个框架采用了面向对象的设计思想，各模块之间的相互影响降到可接收范围
  缺陷：
    1. 内存溢出：未自定义内存管理器，所有的内存分配裸露在外面，一旦处理超过一定限度的数据时会出现内存溢出情况，同时由于内存分配未被封装，因此整个序列化处理器会强制中断，发生段错误且无法修复。
    2. 报错机制：编写.cpp文件时暂未考虑到try catch的报错返回机制，这可能会导致程序出现未能发现的错误时直接崩溃

6/8
1）逐步完成BinarySerialization.cpp的编写，同时初步搭建了使用宏定义各类算术变量以及std容器的接口方法，方便了后续的开发.
2）开发时发现了一个令人困扰的问题：
  两个类乃至三个类的相互调用，在本项目中，用户自定义类BinSerUserDefinedTypeData继承自基类BinSerData，BinSerUserDefinedTypeData实例化时会接收来自用户提供的自定义数据结构，假设为class U，其继承自接口类（纯虚函数）IBinSerUserDefinedType,该接口类一方面要求用户调用含参数宏UserDefinedSerDataParameter(Ser_para) 自定义需要序列化与反序列化的变量，另一方面用做读写函数重载的识别参数。含参数宏UserDefinedSerDataParameter(Ser_para)中重写的函数又以用户自定义类BinSerUserDefinedTypeData为参数

  a.初始状态下 .h文件按照逻辑排序三个类定义顺序如下：
  BinSerData
  BinSerUserDefinedTypeData
  IBinSerUserDefinedType

  b.此时BinSerData读入函数的重载中需要识别IBinSerUserDefinedType类，编译时就会无法找到该类，重排.h顺序如下:
  IBinSerUserDefinedType
  BinSerData
  BinSerUserDefinedTypeData

  c.然而这时候又有一个非常恼人的问题：IBinSerUserDefinedType中重写的函数会以用户自定义类BinSerUserDefinedTypeData为参数，如果故技重施重排.h文件顺序如下
  BinSerUserDefinedTypeData
  IBinSerUserDefinedType
  BinSerData
  
  d.很明显会有一个明显的漏洞：BinSerUserDefinedTypeData继承自BinSerData，而其定义先于BinSerData，即使前置声明BinSerData，BinSerUserDefinedTypeData中也调用了BinSerData中的构造函数，编译会报告不完整类型

  e.然而令人欣慰的是，IBinSerUserDefinedType本身为接口类，其所有函数都为虚函数，而用户再继承自该接口类时显然已在BinSerUserDefinedTypeData类的定义之后，理论上是能够实现的，为此我进行了如下测试模拟上述情况:
  /*****************************************************/
  *  class B;
  *
  *  class A {
  *  public:
  *    virtual void Fun(B* b) = 0;
  *
  *    };
  *
  *  class B {
  *  public:
  *    B(int val): x(val){}
  *    void Print() {
  *      std::cout << x;
  *  }
  *  private:
  *    int x;
  *  };
  *
  *  class C : public A {
  *  public:
  *    void Fun(B* b) {
  *      b->Print();
  *    }
  *  };
  *
  *  int main() {
  *    B* b = new B(3);
  *    C c;
  *    c.Fun(b);
  *  }
  /*****************************************************/
  其中class A即接口类，class B即BinSerUserDefinedTypeData类，class C即用户类，实验发现该方案可成功通过

3)通过上述实验也发现了.h文件中出现的问题，事实上这类问题应该很早就暴露但没有被即使处理。最终重排了.h文件

4)设计初的布尔判断在实际编写.cpp过程中基本懒得使用，一方面是函数相互调用，级联错误的逻辑复杂性较高，另一方面也是代码如果都加入if .. return .. 显得过于臃肿，由此也体现了报错机制的重要性。希望在完成大程序基本功能后加入C++报错机制

5)又发现一个新的问题：
  对于Container类的，使用模板函数获取未知std容器信息时，虽然其size()函数是共有的，然而对于std::map类与std::vector类的获取方式显然无法使用同一模板函数。为此再次更改.h文件加入std容器类型值

6/9
1)继续完成BinarySerialization.cpp的编写
2)出现第一个问题：在BinSerContainerData::ReadSingleType方法的开发中，由于之前读入函数使用的是迭代器获取容器中的数据，然而在读入时需要考虑用户传入的容器本身已经包含数据，container.resize()函数的编写就需要多加考虑，测试代码如下:
/*****************************************************/
*int main() {
*	std::vector<int> t;
*	t.resize(10);
*	int num = t.size();
*	t.resize(10 + t.size());
*	int i = 0;
*	auto it = t.begin();
*	for (;i < num;it++, i++);
*	for (i = 0; it != t.end(); it++, i++) {
*		std::cout << i << std::endl;
*	}
}
/*****************************************************/
3)出现严重问题:模板函数声明和定义分文件写->巨量bug，最终只能用了最丑的方式：将所有的定义都放到了声明下方.........(哭)

6.10
1)学习tinyxml模块