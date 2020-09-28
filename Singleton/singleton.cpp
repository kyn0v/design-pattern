// 单例模式：意图是保证一个类仅有一个实例，并提供一个访问它的全局访问点，该实例被所有程序模块共享
// 定义单例类：
//  1.私有化构造函数，避免外部创建单例类对象
//  2.使用类的私有静态指针变量指向类的唯一实例
//  3.使用一个共有的静态方法获取该实例

// ------------------懒汉版v1（有内存泄漏风险）------------------
// 单例实例在第一次被使用时才进行初始化（延迟初始化）
class Singleton {
   private:
    static Singleton* instance;

   private:
    Singleton(){};
    ~Singleton(){};
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);

   public:
    static Singleton* getInstance() {
        if (instance == nullptr) instance = new Singleton();
        return instance;
    }
};
// init static member
Singleton* Singleton::instance = nullptr;

// ---------------------懒汉版v2（回收内存+DCL）---------------------------
// 回收内存：通过定义嵌套类对应的静态成员，利用程序结束时析构全局变量的特性，释放内存
// 线程安全：这里实现的DCL版本仍存在问题，在于某种情况下会出现new返回了地址赋值给
//          instance变量，而Singleton此时还没有构造完全，当另一个线程随后运行到此处
//          将不会进入if，而导致返回了不完全的实例对象给用户使用，造成了严重的错误。
//          注意到C++11引进了memory model，提供了Atomic实现内存的同步访问（即不同线
//          程总是获取对象修改前或修改后的值，无法在对象修改期间获得该对象），可解决该问题。
class Singleton {
   private:
    static Singleton* instance;

   private:
    Singleton(){};
    ~Singleton(){};
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);

   private:
    class Deletor {
       public:
        ~Deletor() {
            if (Singleton::instance != nullptr) delete Singleton::instance;
        };
    };
    static Deletor deletor;

   public:
    static Singleton* getInstance() {
        if (instance == nullptr) {  // 双检测锁模式（DCL: Double-Checked Locking Pattern）
            Lock lock;              //加锁（RAII）
            if (instance == nullptr) {
                instance = new Singleton();
            }
        }
        return instance;
    }
};
// init static member
Singleton* Singleton::instance = nullptr;

// ---------------------懒汉版v3（Meyers' Singleton）---------------------------
// 使用函数内的 local static 对象，只有第一次访问getInstance()方法时才创建实例
// 并且在c++0x之后该实现是线程安全的
class Singleton {
   private:
    Singleton(){};
    ~Singleton(){};
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);

   public:
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }
};