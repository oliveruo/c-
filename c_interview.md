构造函数、析构函数
！1、构造函数和析构函数
%2、为什么不要在构造器中调用虚函数
%3、为什么不要在析构函数中抛出异常
c++相关
！1、面向对象的三大基本特征，五大基本原则 
%2、C++继承的内存布局 
！3、C++多态的实现机制 
！4、new/deletr和malloc/free的区别
其他
！1、为什么使用补码 
%2、C语言中的内存泄漏 
！3、进制转换
% 4、自己编写strlen/strcpy/strcmp
！ 5、C、C++以及Java之间的区别和各自优缺点



# 1.1  构造函数和析构函数

构造函数的作用：用于新建对象的初始化工作。
 析构函数的作用：用于在撤销对象前，完成一些清理工作，比如：释放内存等。
 每当创建对象时，需要添加初始化代码时，则需要定义自己的构造函数；而对象撤销时，需要自己添加清理工作的代码时，则需要定义自己的析构函数。

在研究C++类的继承、派生、组合时，一直没有清晰地了解构造函数与析构函数的调用过程。本章通过点-线组合类，来深入分析组合类情况下，对象的构造与析构。
1.问题的引入
源代码：
#####include <iostream>  
using namespace std;  

class Point  {   

​     private:  

​      int x;    int y;  

​    public:  

​    int getx();  

​    int gety();  

   Point()  

{ cout<<"Calling the default construction of Point!"<<endl; }  

Point(int xx, int yy)  

{ cout<<"Calling the parameter construction of Point!"<<endl; x = xx; y = yy;}  

Point(Point &pt)  

{ cout<<"Calling the copy construction of Point!"<<endl; x = pt.x;  y = pt.y;}  

~Point()  

{cout<<"Calling the destructor of Point!"<<endl;}  
};  
int Point::getx() { return x;}  
int Point::gety() { return y;}  
//==================================================================  
class Line  
{  
private:  

Point dot1,dot2;  

double Len;  
public:  

Line()  

{cout<<"Calling the default construction of Line!"<<endl;}  

Line(Point pt1, Point pt2)  

{  

cout<<"Calling the parameter construction of  Line"<<endl;  

dot1 = pt1;  dot2 = pt2;  

double Linex = abs( dot1.getx() - dot2.getx() );  

double Liney = abs( dot1.gety() - dot2.gety() );  

Len = sqrt( Linex*Linex + Liney*Liney);  

}  

Line(Line &l)  

{  

cout<<"Calling the copy construction of Line"<<endl;  

Len = l.Len;  

}  

~Line(){ cout<<"Calling the destructor of Line!"<<endl;}  
};  
void main()  
{  

Point mypoint1;   

Point mypoint2(1,2);  

Point mypoint3(mypoint2);  

cout<<mypoint2.getx()<<endl;  

cout<<mypoint3.gety()<<endl;  

Point mypt1(1,2),mypt2(3,4);  

Line line1(mypt1,mypt2);  

Line line2(line1);  

}


程序执行结果：

2.逐条语句进行分析 
Point mypoint1;   
 该条语句是利用Point类进行初始化Point对象，此时，编译器会自动调用程序的构造函数。由于对象定义不带有任何参数，所以此时仅调用程序的默认构造函数。
输出为“Calling the default construction of Point!”
Point mypoint2(1,2);  
语句定义一个Point类的对象，但是此时我们应该注意到对mypoint2对象进行了初始化，此时操作系统会调用含有参数的构造函数。
具体行为过程是，在内存中开辟空间定义临时变量 int xx, int yy;通过参数传递，xx = 1. yy = 2;然后在执行含有参数构造函数的函数体，完成临时变量值向Point类的属性x,y的赋值。
输出“Calling the parameter construction of Point!”
Point mypoint3(mypoint2);  
语句仍然定义了一个Point类的对象，但是应该注意到，这里利用了前一个对象mypoint2对mypoint3进行初始化 。调用了Point类复制构造函数。相当于&pt = mypoint2 。此时，通过看看内存地址，我们可以看到，编译器并没用定义一个Point类的对象临时pt。而是采用了“引用”的模式，pt仅仅是mypoint2的一个别名而已！！！
输出“Calling the copy construction of point!”

cout<<mypoint2.getx()<<endl;  
cout<<mypoint3.gety()<<endl;  
 输出当前Point类的属性x,y;输出“ 1 2”
Point mypt1(1,2),mypt2(3,4);  
定义Point类的两个带有初始化参数的对象,
输出“Calling the parameter construction of Point!”
"Calling the parameter construction of Point!"
[cpp] view plain copy
1.Line line1(mypt1,mypt2);  
注意到，此时我们在尝试利用参数传递的方式来构造Line的对象line1，也就是说，相比较而言Line是上一层的类，而mypt1/mypt2是底层类的对象。此时的程序执行过程可以分解为：
&pt = mypt2;  // “Calling the copy construction of Point!”
&pt = mypt1; // "Calling the copy construction of Point!"
//注意到，此时仍然是引用的格式，没有进行临时对象的构建
line1(mypt1,mypt2); //“Calling the parameter construction of Line!”
重点：当我们利用底层类的对象对上一层类对象进行初始化之后，要立刻销毁底层类的对象，及调用相应的析构函数。
“Calling the destructor of Point!” //销毁对象mypt1
“Calling  the destructor of Point!” //销毁对象mypt2
[cpp] view plain copy
1.Line line2(line1);  
参考上例，此时的拷贝过程可以分解为：
&l = line1; //引用传递
“Calling the default construction of Point!”
"Calling the default construction of Point!"
//执行Line复制构造函数的函数体
“Calling the copy construction of Line”
退出程序后，对所有对象进行销毁，其顺序为“与构造函数调用顺序相反”

# 1.2、为什么不要在构造器中调用虚函数

基类部分在派生类部分之前被构造，当基类构造函数执行时派生类中的数据成员还没被初始化。如果基类构造函数中的虚函数调用被解析成调用派生类的虚函数，而派生类的虚函数中又访问到未初始化的派生类数据，将导致程序出现一些未定义行为和bug

######include <iostream>

class Base
{
public:
    Base() { Foo(); }   ///< 打印 1

    virtual void Foo()
    {
        std::cout << 1 << std::endl;
    }
};

class Derive : public Base
{
public:
    Derive() : Base(), m_pData(new int(2)) {}
    ~Derive() { delete m_pData; }

    virtual void Foo()
    {
        std::cout << *m_pData << std::endl;
    }
private:
    int* m_pData;
};

int main()
{
    Base* p = new Derive();
    delete p;
    return 0;
}
这里的结果将打印：1。
　　这表明第6行执行的的是Base::Foo()而不是Derive::Foo()，也就是说：虚函数在构造函数中“不起作用”。为什么？
　　当实例化一个派生类对象时，首先进行基类部分的构造，然后再进行派生类部分的构造。即创建Derive对象时，会先调用Base的构造函数，再调用Derive的构造函数。
　　当在构造基类部分时，派生类还没被完全创建，从某种意义上讲此时它只是个基类对象。即当Base::Base()执行时Derive对象还没被完全创建，此时它被当成一个Base对象，而不是Derive对象，因此Foo绑定的是Base的Foo。
　　C++之所以这样设计是为了减少错误和Bug的出现。假设在构造函数中虚函数仍然“生效”，即Base::Base()中的Foo();所调用的是Derive::Foo()。当Base::Base()被调用时派生类中的数据m_pData还未被正确初始化，这时执行Derive::Foo()将导致程序对一个未初始化的地址解引用，得到的结果是不可预料的，甚至是程序崩溃（访问非法内存）。
　　总结来说：基类部分在派生类部分之前被构造，当基类构造函数执行时派生类中的数据成员还没被初始化。如果基类构造函数中的虚函数调用被解析成调用派生类的虚函数，而派生类的虚函数中又访问到未初始化的派生类数据，将导致程序出现一些未定义行为和bug。

# 1.3为什么不要在析构函数中抛出异常

C++ 里面当构造函数抛出异常时，其会调用构造函数里面已经创建对象的析构函数，但是对以自己的析构函数没有调用，就可能产生内存泄漏，比如自己new 出来的内存没有释放。
有两个办法。在Catch 块里面释放已经申请的资源 或者 用智能指针把资源当做对象处理。
class Base
{
public:
    void fun()     {   throw 1;    }
    ~Base()        {   throw 2;    }
};

int main()
{
   try
   {
       Base base;
       //base.fun();
   }
   catch (...)
   {
        //cout <<"get the catch"<<endl;
   }
}
运行没有问题。
   实验2: 打开上面注释掉的第13行代码（ //base.fun(); ），再试运行，结果呢？ 在debug 模式下弹出对话框

为什么呢？
因为SEH 是一个链表，链表头地址存在FS:[0] 的寄存器里面。 在实验2，函数base.fun先抛出异常，从FS:[0]开始向上遍历 SHL 节点，匹配到catch 块。 找到代码里面为一个catch块，再去展开栈，调用base 的析构函数，然而析构又抛出异常。 如果系统再去从SEL链表匹配，会改变FS:[0]值，这时候程序迷失了，不知道下面该怎么什么？ 因为他已经丢掉了上一次异常链那个节点。

如果析构函数的异常被处理呢， 程序还会正常运行吗？
class Base
{
public:
    void fun()     {   throw 1;    }
    ~Base()       
    {
        try
        {
              throw 2;   
        }
        catch (int e)
        {
         // do something    
        }    
    }
};

int main()
{
   try
   {
       Base base;
       //base.fun();
   }
   catch (...)
   {
        //cout <<"get the catch"<<endl;
   }
}
的确可以运行。
因为析构抛出来的异常，在到达上一层析构节点之前已经被别的catch 块给处理掉。那么当回到上一层异常函数时， 其SEH 没有变，程序可以继续执行。
这也许就是为什么C++不支持异常中抛的异常

# 2.1面向对象的三大基本特征，五大基本原则 

## 三大特性是：封装,继承,多态  

所谓封装，也就是把客观事物封装成抽象的类，并且类可以把自己的数据和方法只让可信的类或者对象操作，对不可信的进行信息隐藏。封装是面向对象的特征之一，是对象和类概念的主要特性。 简单的说，一个类就是一个封装了数据以及操作这些数据的代码的逻辑实体。在一个对象内部，某些代码或某些数据可以是私有的，不能被外界访问。通过这种方式，对象对内部数据提供了不同级别的保护，以防止程序中无关的部分意外的改变或错误的使用了对象的私有部分。

所谓继承是指可以让某个类型的对象获得另一个类型的对象的属性的方法。它支持按级分类的概念。继承是指这样一种能力：它可以使用现有类的所有功能，并在无需重新编写原来的类的情况下对这些功能进行扩展。 通过继承创建的新类称为“子类”或“派生类”，被继承的类称为“基类”、“父类”或“超类”。继承的过程，就是从一般到特殊的过程。要实现继承，可以通过“继承”（Inheritance）和“组合”（Composition）来实现。继承概念的实现方式有二类：实现继承与接口继承。实现继承是指直接使用基类的属性和方法而无需额外编码的能力；接口继承是指仅使用属性和方法的名称、但是子类必须提供实现的能力；

所谓多态就是指一个类实例的相同方法在不同情形有不同表现形式。多态机制使具有不同内部结构的对象可以共享相同的外部接口。这意味着，虽然针对不同对象的具体操作不同，但通过一个公共的类，它们（那些操作）可以通过相同的方式予以调用。

## 五大基本原则 

### 单一职责原则SRP(Single Responsibility Principle)

是指一个类的功能要单一，不能包罗万象。如同一个人一样，分配的工作不能太多，否则一天到晚虽然忙忙碌碌的，但效率却高不起来。

### 开放封闭原则OCP(Open－Close Principle) 

一个模块在扩展性方面应该是开放的而在更改性方面应该是封闭的。比如：一个网络模块，原来只服务端功能，而现在要加入客户端功能，
那么应当在不用修改服务端功能代码的前提下，就能够增加客户端功能的实现代码，这要求在设计之初，就应当将服务端和客户端分开，公共部分抽象出来。

### 替换原则(the Liskov Substitution Principle LSP) 

子类应当可以替换父类并出现在父类能够出现的任何地方。比如：公司搞年度晚会，所有员工可以参加抽奖，那么不管是老员工还是新员工，
也不管是总部员工还是外派员工，都应当可以参加抽奖，否则这公司就不和谐了。

### 依赖原则(the Dependency Inversion Principle DIP) 

具体依赖抽象，上层依赖下层。假设B是较A低的模块，但B需要使用到A的功能，

这个时候，B不应当直接使用A中的具体类： 而应当由B定义一抽象接口，并由A来实现这个抽象接口，B只使用这个抽象接口：这样就达到
了依赖倒置的目的，B也解除了对A的依赖，反过来是A依赖于B定义的抽象接口。通过上层模块难以避免依赖下层模块，假如B也直接依赖A的实现，那么就可能造成循环依赖。一个常见的问题就是编译A模块时需要直接包含到B模块的cpp文件，而编译B时同样要直接包含到A的cpp文件。

### 接口分离原则(the Interface Segregation Principle ISP) 

模块间要通过抽象接口隔离开，而不是通过具体的类强耦合起来

# 2.2 C++继承的内存布局 

先看个首先我们考虑一个（非虚拟）多重继承的相对简单的例子。看看下面的C++类层次结构。 

class Top 

{

  public: int a;

 };

  class Left : public Top 

{  

public: int b;

 };  

class Right : public Top 

{

  public: int c;

 };

  class Bottom : public Left, public Right

 {

  public: int d;

 }; 

![这里写图片描述](https://img-blog.csdn.net/20160128151123315)

注意Top被继承了两次（在Eiffel语言中这被称作重复继承）。这意味着类型Bottom的一个实例bottom将有两个叫做a的元素（分别为bottom.Left::a和bottom.Right::a）。

Left、Right和Bottom在内存中是如何布局的？让我们先看一个简单的例子。Left和Right拥有如下的结构：

| **Left** | **Right** |
| -------- | --------- |
| Top::a   | Top::a    |
| Left::b  | Right::c  |

请注意第一个属性是从Top继承下来的。这意味着在下面两条语句后

```
Left* left = new Left();
Top* top = left;12
```

left和top指向了同一地址，我们可以把Left Object当成Top Object来使用(很明显，Right与此也类似)。那Buttom呢？GCC的建议如下：

| **Bottom**    |
| ------------- |
| Left::Top::a  |
| Left::b       |
| Right::Top::a |
| Right::c      |
| Bottom::d     |

如果我们提升Bottom指针，会发生什么事呢？

```
Bottom* bottom = new Bottom();
Left* left = bottom;12
```

这段代码工作正常。我们可以把一个Bottom的对象当作一个Left对象来使用，因为两个类的内存部局是一样的。那么，如果将其提升为Right呢？会发生什么事？

```
Right* right = bottom;1
```

为了执行这条语句，我们需要判断指针的值以便让它指向Bottom中对应的段。

| **Bottom**                  |
| --------------------------- |
| Left::Top::a                |
| Left::b                     |
| Right::Top::a <—rightpoints |
| Right::c                    |
| Bottom::d                   |

经过这一步，我们可以像操作正常Right对象一样使用right指针访问bottom。虽然，bottom与right现在指向两个不同的内存地址。出于完整性的缘故，思考一下执行下面这条语句时会出现什么状况。

```
Top* top = bottom;1
```

是的，什么也没有。这条语句是有歧义的：编译器将会报错。

```
error: `Top' is an ambiguous base of `Bottom'1
```

两种方式可以避免这样的歧义

```
Top* topL = (Left*) bottom;
Top* topR = (Right*) bottom;12
```

执行这两条语句后，topL和left会指向同样的地址，topR和right也会指向同样的地址。

# 虚拟继承

为了避免重复继承Top，我们必须虚拟继承Top：

```
class Top
{ 
    public: int a;
}; 
class Left : virtual public Top
{ 
    public: int b;
}; 
class Right : virtual public Top
{ 
    public: int c;
}; 
class Bottom : public Left, public Right
{ 
    public: int d;
};12345678910111213141516
```

这就得到了如下的层次结构（也许是你一开始就想得到的） 
![这里写图片描述](https://img-blog.csdn.net/20160128201653149) 
虽然从程序员的角度看，这也许更加的明显和简便，但从编译器的角度看，这就变得非常的复杂。重新考虑下Bottom的布局，其中的一个（也许没有）可能是：

| **Bottom**   |
| ------------ |
| Left::Top::a |
| Left::b      |
| Right::c     |
| Bottom::d    |

这个布局的优点是，布局的第一部分与Left的布局重叠了，这样我们就可以很容易的通过一个Left指针访问 Bottom类。可是我们怎么处理

```
Right* right = bottom;1
```

我们将哪个地址赋给right呢? 经过这个赋值，如果right是指向一个普通的Right对象，我们应该就能使用 right了。但是这是不可能的！Right本身的内存布局是完全不同的，这样我们就无法像访问一个”真正的”Right对象一样，来访问升级的Bottom对象。而且，也没有其它（简单的）可以正常运作的Bottom布局。 
解决办法是复杂的。我们先给出解决方案，之后再来解释它。

![这里写图片描述](https://img-blog.csdn.net/20160128201848245)

你应该注意到了这个图中的两个地方。第一，字段的顺序是完全不同的（事实上，差不多是相反的）。第二，有几个vptr指针。这些属性是由编译器根据需要自动插入的（使用虚拟继承，或者使用虚拟函数的时候）。编译器也在构造器中插入了代码，来初始化这些指针。

vptr (virtual pointers)指向一个 “虚拟表”。类的每个虚拟基类都有一个vptr指针。要想知道这个虚拟表 (vtable)是怎样运用的，看看下面的C++ 代码。

```
Bottom* bottom = new Bottom();
Left* left = bottom; 
int p = left->a;123
```

第二个赋值使left指向了bottom的所在地址（即，它指向了Bottom对象的“顶部”）。我们想想最后一条赋值语句的编译情况（稍微简化了）：

```
movl left, %eax # %eax = left
movl (%eax), %eax # %eax = left.vptr.Left
movl (%eax), %eax # %eax = virtual base offset
addl left, %eax # %eax = left + virtual base offset
movl (%eax), %eax # %eax = left.a
movl %eax, p # p = left.a123456
```

用语言来描述的话，就是我们用left指向虚拟表，并且由它获得了“虚拟基类偏移”(vbase)。这个偏移之后就加到了left，然后left就用来指向Bottom对象的Top部分。从这张图你可以看到Left的虚拟基类偏移是20；如果假设Bottom中的所有字段都是4个字节，那么给left加上20字节将会确实指向a字段。

经过这个设置，我们就可以同样的方法访问Right部分。按这样

```
Bottom* bottom = new Bottom();
Right* right = bottom; 
int p = right->a;123
```

之后right将指向Bottom对象的合适的部位：

| Bottom             |
| ------------------ |
| vptr.Left          |
| Left::b            |
| vptr.Right <—right |
| Right::c           |
| Bottom::d          |
| Top::a             |

对top的赋值现在可以编译成像前面Left同样的方式。唯一的不同就是现在的vptr是指向了虚拟表的不同部位：取得的虚拟表偏移是12，这完全正确（确定！）。我们可以将其图示概括：

![这里写图片描述](https://img-blog.csdn.net/20160128202152609)

当然，这个例子的目的就是要像访问真正Right对象一样访问升级的Bottom对象。因此，我们必须也要给Right（和Left）布局引入vptrs：

![这里写图片描述](https://img-blog.csdn.net/20160128202235704)

现在我们就可以通过一个Right指针，一点也不费事的访问Bottom对象了。不过，这是付出了相当大的代价：我们要引入虚拟表，类需要扩展一个或更多个虚拟指针，对一个对象的一个简单属性的查询现在需要两次间接的通过虚拟表（即使编译器某种程度上可以减小这个代价）。

# 向下转换

如我们所见，将一个派生类的指针转换为一个父类的指针(或者说，向上转换)可能涉及到给指针增添一个偏移。有人可能会想了，这样向下转换（反方向的）就可以简单的通过减去同样的偏移来实现。确实，对非虚拟继承来说是这样的。可是，虚拟继承（毫不奇怪的！）带来了另一种复杂性。

假设我们像下面这个类这样扩展继承层次。

```
class AnotherBottom : public Left, public Right
{ 
    public: int e; int f;
};1234
```

继承层次现在看起来是这样

![这里写图片描述](https://img-blog.csdn.net/20160128202337096)

现在考虑一下下面的代码。

```
Bottom* bottom1 = new Bottom();
AnotherBottom* bottom2 = new AnotherBottom();
Top* top1 = bottom1;
Top* top2 = bottom2;
Left* left = static_cast<Left*>(top1);12345
```

下图显示了Bottom和AnotherBottom的布局，而且在最后一个赋值后面显示了指向top的指针。

![这里写图片描述](https://img-blog.csdn.net/20160128202445770)

现在考虑一下怎么去实现从top1到left的静态转换，同时要想到，我们并不知道top1是否指向一个Bottom类型的对象，或者是指向一个AnotherBottom类型的对象。所以这办不到！这个重要的偏移依赖于top1运行时的类型（Bottom则20，AnotherBottom则24）。编译器将报错：

```
error: cannot convert from base `Top' to derived type `Left'
via virtual base `Top'12
```

因为我们需要运行时的信息，所以应该用一个动态转换来替代实现：

```
Left* left = dynamic_cast<Left*>(top1);1
```

可是，编译器仍然不满意：

```
error: cannot dynamic_cast `top' (of type `class Top*') to type
   `class Left*' (source type is not polymorphic)12
```

（注：polymorphic多态的）

问题在于，动态转换（转换中使用到typeid）需要top1所指向对象的运行时类型信息。但是，如果你看看这张图，你就会发现，在top1指向的位置，我们仅仅只有一个integer (a)而已。编译器没有包含指向Top的虚拟指针，因为它不认为这是必需的。为了强制编译器包含进这个vptr指针，我们可以给Top增加一个虚拟的析构器：

```
class Top
{ 
public: 
    virtual ~Top() {}
    int a;
};123456
```

这个修改需要指向Top的vptr指针。Bottom的新布局是

![这里写图片描述](https://img-blog.csdn.net/20160128202702630)

(当然类似的其它类也有一个新的指向Top的vptr指针)。现在编译器为动态转换插进了一个库调用：

```
left = __dynamic_cast(top1, typeinfo_for_Top, typeinfo_for_Left, -1);1
```

这个函数__dynamic_cast定义在stdc++库中(相应的头文件是cxxabi.h)；参数为Top的类型信息，Left和Bottom(通过vptr.Top)，这个转换可以执行。 (参数 -1 标示出Left和Top之间的关系现在还是未知)。更多详细资料，请参考tinfo.cc 的具体实现 。

# 总结语

最后，我们来看看一些没了结的部分。

## 指针的指针

这里出现了一点令人迷惑的问题，但是如果你仔细思考下一的话它其实很简单。我们来看一个例子。假设使用上一节用到的类层次结构(向下类型转换).在前面的小节我们已经看到了它的结果：

```
Bottom* b = new Bottom();
Right* r = b;12
```

(在将b的值赋给r之前，需要将它调整8个字节，从而让它指向Bottom对象的Right部分).因此，我们可以合法地将一个Bottom* 赋值给一个Right*的指针。但是Bottom**和Right**又会怎样呢？

```
Bottom** bb = &b;
Right** rr = bb;12
```

编译器会接受这样的形式吗？我们快速测试一下，编译器会报错：

```
error: invalid conversion from `Bottom**' to `Right**'1
```

为什么呢？假设编译器可以接受从bb到rr的赋值。我们可以只管的看到结果如下：

![这里写图片描述](https://img-blog.csdn.net/20160128202859615)

因此，bb和rr都指向b，并且b和r指向Bottom对象的正确的章节。现在考虑当我们赋值给*rr时会发生什么(注意\*rr的类型时Right*,因此这个赋值是有效的):

```
*rr = b;   1
```

这样的赋值和上面的赋值给r在根本上是一致的。因此，编译器会用同样的方式实现它！特别地，它会在赋值给*rr之前将b的值调整8个字节。办事*rr指向的是b！我们再一次图示化这个结果：

![这里写图片描述](https://img-blog.csdn.net/20160128202946746)

只要我们通过*rr来访问Bottom对象这都是正确的，但是只要我们通过b自身来访问它，所有的内存引用都会有8个字节的偏移—明显这是个不理想的情况。 
因此，总的来说，及时*a 和*b通过一些子类型相关，**aa和**bb却是不相关的。

## 虚拟基类的构造函数

编译器必须确保对象的所有虚指针都被正确的初始化。特别是，编译器确保了类的所有虚基类都被调用，并且只被调用一次。如果你不显示地调用虚拟超类(不管他们在继承层次结构中的距离有多远)，编译器都会自动地插入调用他们缺省构造函数。 
这样也会引来一些不可以预期的错误。以上面给出的类层次结构作为示例，并添加上构造函数的部分：

```
class Top
{ 
public:
    Top() 
    { 
        printf("Top()\n");
        a = -1; 
    }
    Top(int _a) 
    { 
        printf("Top(int _a=%d)\n", _a);
        a = _a; 
    } 
    int a;
}; 

class Left : virtual public Top
{ 
public:
    Left() 
    {
        printf("Left()\n");
        b = -2; 
    }
    Left(int _a, int _b) : Top(_a) 
    { 
        printf("Left(int _a=%d, int _b=%d)\n", _a, _b);
        b = _b; 
    } 
    int b;
}; 

class Right : virtual public Top
{ 
public:
    Right() 
    { 
        printf("Right()\n");
        c = -3; 
    }
    Right(int _a, int _c) : Top(_a) 
    { 
        printf("Right(int _a=%d, int _c=%d)\n", _a, _c);
        c = _c; 
    } 
    int c;
}; 

class Bottom : public Left, public Right
{ 
public:
    Bottom() 
    { 
        printf("Bottom()\n");
        d = -4; 
    }
    Bottom(int _a, int _b, int _c, int _d) : Left(_a, _b), Right(_a, _c)
    {
        printf("Bottom(int _a=%d, int _b=%d, int _c=%d, int _d=%d)\n", _a, _b, _c, _d);
        d = _d;
    } 
    int d;
};123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263
```

(首先考虑非虚拟的情况。)你会期望下面的代码段输出什么：

```
Bottom bottom(1,2,3,4);
printf("%d %d %d %d %d\n", bottom.Left::a, bottom.Right::a,
   bottom.b, bottom.c, bottom.d);123
```

你可能会希望得到下面的结果，并且也得到了下面的结果：

```
1 1 2 3 41
```

然而，现在考虑虚拟的情况(我们虚拟继承自Top类)。如果我们仅仅做那样一个改变，并再一次运行程序，我们会得到：

```
 -1 -1 2 3 41
```

为什么呢？通过跟踪构造函数的执行，会发现：

```
Top::Top()
Left::Left(1,2)
Right::Right(1,3)
Bottom::Bottom(1,2,3,4)1234
```

就像上面解释的一样，编译器在Bottom类执行其他构造函数之前中插入调用了缺省构造函数。 然后，当Left去调用它自身的超类的构造函数时(Top),我们会发现Top已经被初始化了因此构造函数不会被调用。 
为了避免这种情况，你应该显示的调用虚基类的构造函数：

```
Bottom(int _a, int _b, int _c, int _d): Top(_a), Left(_a,_b), Right(_a,_c)
{
   d = _d;
}1234
```

## 指针等价

再假设同样的（虚拟）类继承等级，你希望这样就打印“相等”吗？

```
Bottom* b = new Bottom();
Right* r = b; if(r == b)
   printf("Equal!\n");123
```

记住这两个地址并不实际相等（r偏移了8个字节）。但是这应该对用户完全透明；因此，实际上编译器在r与b比较之前，就给r减去了8个字节；这样，这两个地址就被认为是相等的了。

## 转换为void类型的指针

最后，我们来思考一下当将一个对象转换为void类型的指针时会发生什么事情。编译器必须保证一个指针转换为void类型的指针时指向对象的顶部。使用虚函数表这很容易实现。你可能已经想到了指向top域的偏移量是什么。它是虚函数指针到对象顶部的偏移量。因此，转化为void类型的指针操作可以使用查询虚函数表的方式来实现。然而一定要确保使用动态类型转换，如下：

```
dynamic_cast<void*>(b);
```

# 2.3C++多态的实现机制 

多态的实现表象是指针+虚函数，本质是虚表+虚指针。
1.虚表
声明了虚函数的类会隐式创建一个虚指针指向虚表，而虚表保存了虚函数的地址(虚表中只有虚函数，非虚函数在另一个表中)，当我们调用虚函数时，实际上是通过虚指针去查找虚表中对应虚函数的地址
2.虚指针
虚指针指向类的虚表，属于类的成员变量，可以继承，当子类继承父类的虚指针时，子类的虚指针指向子类的虚表。虚表也可以继承，也可以理解为继承了虚函数，然后创建了自己的虚表，所以如果没有重定义父类虚函数，那么子类虚表和父类虚表完全相同；重定义的虚函数在子类虚表中的地址会改变。
3.虚指针的初始化
虚指针在构造函数中进行初始化，指向类的虚表。如果类B继承了类A，那么我们知道生成B的实例对象时，会先调用A的构造函数，此时虚指针指向A的虚表，然后调用B的构造函数，此时虚指针指向B的虚表，所以当B的实例构造完成后，虚指针指向B的虚表。
4.多态
 对于虚函数调用来说，每一个对象内部都有一个虚表指针，该虚表指针被初始化为本类的虚表。所以在程序中，不管你的对象类型如何转换，但该对象内部的虚表指针是固定的，所以呢，才能实现动态的对象函数调用，这就是C++多态性实现的原理。
用下面的例子讲解，当我们声明一个父类A的指针p指向子类B的对象时，在编译阶段是不分配内存的(不构造对象的)，也就是说编译器并不知道指针p指向的是B类的对象，只知道这是一个A类的指针，那么在编译p->func()时，会直接查找A中func的地址并替换(可以简单理解为这样)，也就是静态绑定，那么在运行时自然调用的就是A的func了
class A
{
public:
    A(){};
    void func()
    {
        cout << "A func called!";
    }
};

class B:public A
{
public:
    B(){};
    void func()
    {
        cout << "B func called!";
    }

};

int main()
{
    A*p = new B();
    p->func();
    return 0;
}
但是如果将func声明为虚函数，如下，那么在编译时编译器一看func是虚函数，会直接跳过，那么在运行时，B的对象已经被构造出来了，那么p所指向的B对象的虚指针已经指向了B的虚表，此时调用p->func()时，操作系统会在p的虚表中去寻找func这个函数，也就是动态绑定，然后调用，这时候调用的自然就是B的func了，可以说虚指针和虚表就是为了多态而生的。
class A
{
public:
    A(){};
    virtual  void func()
    {
        cout << "A func called!";
    }
};

class B:public A
{
public:
    B(){};
    void func()
    {
        cout << "B func called!";
    }

};

int main()
{
    A*p = new B();
    p->func();
    return 0;
}

# 2.4new/deletr和malloc/free的区别

相同点：都可用于申请动态内存和释放内存
不同点：
（1）操作对象有所不同。
malloc与free是C++/C 语言的标准库函数，new/delete 是C++的运算符。对于非内部数据类的对象而言，光用maloc/free 无法满足动态对象的要求。对象在创建的同时要自动执行构造函数， 对象消亡之前要自动执行析构函数。由于malloc/free 是库函数而不是运算符，不在编译器控制权限之内，不能够把执行构造函数和析构函数的任务强加malloc/free。
（2）用法上也有所不同。
函数malloc 的原型如下：
void * malloc(size_t size);
用malloc 申请一块长度为length 的整数类型的内存，程序如下：
int *p = (int *) malloc(sizeof(int) * length);
我们应当把注意力集中在两个要素上：“类型转换”和“sizeof”。
1、malloc 返回值的类型是void *，所以在调用malloc 时要显式地进行类型转换，将void * 转换成所需要的指针类型。
2、 malloc 函数本身并不识别要申请的内存是什么类型，它只关心内存的总字节数。
函数free 的原型如下：
void free( void * memblock );
为什么free 函数不象malloc 函数那样复杂呢？这是因为指针p 的类型以及它所指的内存的容量事先都是知道的，语句free(p)能正确地释放内存。如果p 是NULL 指针，那么free
对p 无论操作多少次都不会出问题。如果p 不是NULL 指针，那么free 对p连续操作两次就会导致程序运行错误。

new/delete 的使用要点：
运算符new 使用起来要比函数malloc 简单得多，例如：
int *p1 = (int *)malloc(sizeof(int) * length);
int *p2 = new int[length];
这是因为new 内置了sizeof、类型转换和类型安全检查功能。对于非内部数据类型的对象而言，new 在创建动态对象的同时完成了初始化工作。如果对象有多个构造函数，那么new 的语句也可以有多种形式。
如果用new 创建对象数组，那么只能使用对象的无参数构造函数。例如
Obj *objects = new Obj[100];       // 创建100 个动态对象
不能写成
Obj *objects = new Obj[100](1);        // 创建100 个动态对象的同时赋初值1
在用delete 释放对象数组时，留意不要丢了符号‘[]’。例如
delete []objects; // 正确的用法
delete objects; // 错误的用法
后者相当于delete objects[0]，漏掉了另外99 个对象。
  1、new自动计算需要分配的空间，而malloc需要手工计算字节数
  2、new是类型安全的，而malloc不是，比如：
             int* p = new float[2]; // 编译时指出错误
             int* p = malloc(2*sizeof(float)); // 编译时无法指出错误
          new operator 由两步构成，分别是 operator new 和 construct
   3、operator new对应于malloc，但operator new可以重载，可以自定义内存分配策略，甚至不做内存分配，甚至分配到非内存设备上。而malloc无能为力
   4、new将调用constructor，而malloc不能；delete将调用destructor，而free不能。
   5、malloc/free要库文件支持，new/delete则不要。 

## 1、本质区别

malloc/free是C/C++语言的标准库函数，new/delete是C++的运算符。
对于用户自定义的对象而言，用maloc/free无法满足动态管理对象的要求。对象在创建的同时要自动执行构造函数，对象在消亡之前要自动执行析构函数。由于malloc/free是库函数而不是运算符，不在编译器控制权限之内，不能够把执行构造函数和析构函数的任务强加于malloc/free。因此C++需要一个能完成动态内存分配和初始化工作的运算符new，以及一个能完成清理与释放内存工作的运算符delete。
class Obj  
{  
public:  

Obj( )   

{ cout  <<  "Initialization"  <<  endl; }  

~ Obj( )  

{ cout  <<  "Destroy" <<  endl; }  

void Initialize( )  

{ cout  <<  "Initialization"  <<  endl; }  

void  Destroy( )  

{ cout  <<  "Destroy"  <<  endl; }  
}obj;  


void  UseMallocFree( )  
{  

Obj   * a  =  (Obj  *) malloc( sizeof ( obj ) );      //  allocate memory   

a -> Initialize();                                    //  initialization    

a -> Destroy();                                        // deconstruction   

free(a);                                               // release memory  

}  


void  UseNewDelete( void )  
{  

Obj   * a  =   new  Obj;                                             

delete a;   
}  
类Obj的函数Initialize实现了构造函数的功能，函数Destroy实现了析构函数的功能。函数UseMallocFree中，由于malloc/free不能执行构造函数与析构函数，必须调用成员函数Initialize和Destroy来完成“构造”与“析构”。所以我们不要用malloc/free来完成动态对象的内存管理，应该用new/delete。由于内部数据类型的“对象”没有构造与析构的过程，对它们而言malloc/free和new/delete是等价的。

## 2、联系

既然new/delete的功能完全覆盖了malloc/free，为什么C++还保留malloc/free呢？因为C++程序经常要调用C函数，而C程序只能用malloc/free管理动态内存。如果用free释放“new创建的动态对象”，那么该对象因无法执行析构函数而可能导致程序出错。如果用delete释放“malloc申请的动态内存”，理论上讲程序不会出错，但是该程序的可读性很差。所以new/delete、malloc/free必须配对使用。

# 3.1为什么使用补码 

我们都知道计算机中的负数是用补码来表示的，而负数的补码是原码符号位不变，其他位按位取反再加一。但是为什么必须这样？为什么非要取反再+1？这个定义是怎么来的？
首先我们用我们熟悉的十进制来思考问题，假设现在我们只考虑两位数字的运算，比如56 + (-28) = 56 - 28，这里如果作正常的减法运算，就需要借位，也就是先让"6"减去"8"的时候，发现不够减，所以要向高位“借一位”，所以"5"只能借一位给“6”，然后“5”变成“4”，这样的“借位规则”如果用电路来实现，会很复杂（至于如何用电路实现，感兴趣的同学可以去看《编码》的一到十二章)，但幸运的是我们有办法来避免这样的借位：
56 + (-28) = 56 - 28 + 99 - 99       //加99再减99表达式值不变
           = 56 - 28 + 99 + 1 - 100  //把-99写成1 - 100
           = (99 - 28 + 1) + 56 - 100  //交换一下位置         
如上面最后一步所示，如果按照其从左到右的顺序依次运算，是完全不需要借位的，这里的99其实可以看成“两位数字能表示的最大的数值“，如果是三位数字，那就是999。最后在减去100之前，其实已经”溢出“了，这时候只要把最高位舍去就行了。
换到二进制也是一个道理，假设我们只能用八位数字来表示一个数值，那么上面的28写成二进制就是00011100，然后我们要用“八位数字能表示的最大的数值”减去它，就是11111111 - 00011100，这时候你会发现，这样做的结果刚好就是00011100全部都取反一下（0变1,1变0），变成11100011，然后再加一变成11100100，这时候再加上56（也就是二进制的00111000）:
    11100100


​    00111000

------------
   100011100
然后把结果中多出来的一位最高位舍去，就得到了00011100，也就是十进制的28（56 - 28 = 28）！
写到这里我想大家应该已经明白了，只要把-28（二进制的-00011100）用72（二进制的11100100）来表示，就能直接用加法得到正确结果了。这也是为什么负数的符号位是1的原因，因为，比如用八位二进制来表示一个数值，如果要包含负数的话，那我们只能舍弃一半原来的正数，让这部分正数来表示负数，很自然地我们用“更大的那一半”来表示负数，比如原来可以表示0到256，现在我们就把128到256“分给负数”，剩下的0到127继续表示正数（如果用128到256来表示正数，0到127表示负数，那么0到127和-127到0这部分数值就没人表示了）。所以，“一串01”在计算机中到底表示几，还得看它能不能表示负数。

# 3.2C语言中的内存泄漏 

 (1)内存申请未成功，然后进行使用；//在编程时经常用if (p == NULL) 进行判断；
 (2)内存申请成功，但是没有初始化，会造成内存出错；
 (3)内存初始化成功，但是操作越界，比如在数组的操作当中加一；char a [5] = "hello";会造成段错误,没有考虑到'\0'的存储空间，若越界访问的内存空间是空闲的，则程序可能不受影响。若空间已经被占用，若执行了非法操作，程序可能奔溃。
(4)忘记释放内存或者释放一部分则会造成内存泄露。

# 3.3进制转换

## 任意进制转换为10进制； 

long  int toTen(char a[], int bit)
		 {
		     int length = strlen(a);
		     int i, b = 1, sum = 0; //i要做数组a的下标，注意其起止范围 
		     for (i = length - 1; i >= 0; i--)
			     {
			         if (a[i] >= 'A')
				         {
				             sum += (a[i] - 'A' + 10) *b;
				             b *= bit;
				         }
			         else
				         {
				             sum += (a[i] - '0') *b;
				             b *= bit;
				         }
			     }
		     return sum;
		 }

## 十进制转换为任意进制（利用栈实现）

```
#include<iostream>
#include<stack>
using namespace std;

int main()
{
    int r,n;
    stack<int> s;
    cin>>n>>r;
    while(n)
    {
        s.push(n%r);
        n/=r;
    }
    while(!s.empty())
    {
        switch(s.top())
        {
            case 10:cout<<'A';break;
            case 11:cout<<'B';break;
            case 12:cout<<'C';break;
            case 13:cout<<'D';break;
            case 14:cout<<'E';break;
            case 15:cout<<'F';break;//这些都是为了能转化为十六进制。
            default:cout<<s.top();break;
        }
        s.pop();
    }
    return 0;
 } 

```

##  任意进制转换成任意进制

int main()
{
	char s1[] = "E";
	int a = toTen(s1, 16);//利用前面10进制转换成任意进制
	cout << a << endl;
	char temp[200];
	int b= 15;
	_itoa(a, temp, 16);//能够将十进制的int型转换成任意进制的字符串输出 
	cout << temp;

​        system("pause");
	return 0;

}

# 3.4自己编写strlen/strcpy/strcmp/strcat 

###  一、字符串拷贝strcpy

函数`strcpy`的原型是`char* strcpy(char* des , const char* src)`，des 和 src 所指内存区域不可以重叠且 des 必须有足够的空间来容纳 src 的字符串。

#####    #include <assert.h>  

\#include <stdio.h>  

char* strcpy(char* des, const char* src)  

{  

​    assert((des!=NULL) && (src!=NULL));   

​    char *address = des;    

​    while((*des++ = *src++) != '\0')    

​        ;    

​    return address;  

}  

要知道 strcpy 会拷贝'\0'，还有要注意：

- 源指针所指的字符串内容是不能修改的，因此应该声明为 const 类型。
- 要判断源指针和目的指针为空的情况，思维要严谨，这里使用`assert`（见文末）。
- 要用一个临时变量保存目的串的首地址，最后返回这个首地址。
- 函数返回 char* 的目的是为了支持链式表达式，即strcpy可以作为其他函数的实参。

### 二、字符串长度strlen

函数`strlen`的原型是`size_t strlen(const char *s)`，其中 size_t 就是 unsigned int。

#######include <assert.h>  

\#include <stdio.h>  

int strlen(const char* str)  

{  

​    assert(str != NULL);  

​    int len = 0;  

​    while((*str++) != '\0')  

​        ++len;  

​    return len;  

}  

strlen 与 sizeof 的区别：

- sizeof是运算符，strlen是库函数。
- sizeof可以用类型、变量做参数，而strlen只能用 char* 变量做参数，且必须以`\0`结尾。
- sizeof是在编译的时候计算类型或变量所占内存的大小，而strlen的结果要在运行的时候才能计算出来，用来计算字符串的长度。
- 数组做sizeof的参数不退化，传递给strlen就退化为指针了。

### 三、字符串连接strcat

函数`strcat`的原型是`char* strcat(char* des, char* src)`，des 和 src 所指内存区域不可以重叠且 des 必须有足够的空间来容纳 src 的字符串。

\#include <assert.h>  

\#include <stdio.h>    

char* strcat(char* des, const char* src)   // const表明为输入参数   

{    

​    assert((des!=NULL) && (src!=NULL));  

​    char* address = des;  

​    while(*des != '\0')  // 移动到字符串末尾  

​        ++des;  

​    while(*des++ = *src++)  

​        ;  

​    return address;  

}   

### 四、字符串比较strcmp

函数`strcmp`的原型是`int strcmp(const char *s1,const char *s2)`。

- 若s1==s2，返回零；
- 若s1>s2，返回正数；
- 若s1<s2，返回负数。

即：两个字符串自左向右逐个字符相比（按ASCII值大小相比较），直到出现不同的字符或遇`\0`为止。

\#include <assert.h>  

\#include <stdio.h>  

int strcmp(const char *s1,const char *s2)  

{  

​    assert((s1!=NULL) && (s2!=NULL));  

​    while(*s1 == *s2)  

​    {  

​        if(*s1 == '\0')  

​            return 0;             

​        ++s1;  

​        ++s2;  

​    }  

​    return *s1 - *s2;  

}   

### 附：assert()断言(调试常用)

`assert`是宏，而不是函数。它的原型定义在头文件 assert.h 中：

1. void assert( int expression );  

宏 assert 经常用于在函数开始处检验传入参数的合法性，可以将其看作是异常处理的一种高级形式。**assert 的作用是**先计算表达式expression，然后判断：

- 如果表达式值为假，那么它先向stderr打印错误信息，然后通过调用 abort 来终止程序运行。
- 如果表达式值为真，继续运行后面的程序。

注意：`assert`只在 DEBUG 下生效，在调试结束后，可以通过在`#include <assert.h>`语句之前插入`#define NDEBUG`来禁用assert调用。

# 5.C、C++以及Java之间的区别和各自优缺点

C/C++：
           
C/C++代码——编译（不同的系统编译出不同的机器码，所以同一个C/C++文件不一定可以在某些系统执行，因为编译出的机器码不同）——机器码————在操作系统中由硬盘读取到内存中运行——内存——CPU——输出结果


Java：             
Java代码————编译得到字节码文件(.class)————JVM执行字节码文件（字节码在虚拟机上运行，虚拟机相当于翻译官，不同的系统JVM不同，转换规则不同，把同一个字节码文件转换为相应的系统的机器码）————机器码在相应系统运行——内存——CPU——结果


由于JVM的存在，只需在不同的系统上安装相应的JVM，同一个.class文件在相应的系统的JVM运行就会输出相应系统能解析的机器码，从而成功运行。
这就是，一次编译，到处运行。
打个比方，就是：
一本汉字写的书（源码），去到不同的国家（系统），每个国家有相应的翻译官（JVM虚拟机）把汉字翻译成其所在国家的文字（比如这本书传到英国被翻译成英文书），之后就可以在这个国家流传开了（相当于程序成功运行）。

另外，补充一点： 
JAVA有两特性：
移植性：一次编译，到处运行（上面已解释）
安全性：自动回收内存中不常用的命令垃圾，防止内存溢出。

参考：https://www.cnblogs.com/carter2000/archive/2012/04/28/2474960.html
           https://www.cnblogs.com/zhyg6516/archive/2011/03/08/1977007.html
           https://blog.csdn.net/melody_1016/article/details/52966355
           https://blog.csdn.net/zyq0335/article/details/7657465
           https://www.cnblogs.com/raytheweak/p/7290617.html
           https://blog.csdn.net/pengfeiwen/article/details/2765897 

​           https://blog.csdn.net/melody_1016/article/details/52966355

​           https://www.oschina.net/translate/cpp-virtual-inheritance

​           https://blog.csdn.net/lisonglisonglisong/article/details/44278013

​       《编码》

​       《深入理解c指针》