

namespace pheet {

template<typename TT>
class AtomicStampedReference{

private:
	int *stampHolder;

public:
	bool compareAndSet(TT expectedReference, TT newReference, int expectedStamp, int newStamp){
		bool ret;

		return ret;
	}

public:
	TT get(int stampHolder[]){
		TT ret;

		return ret;
	}

public:
	void set(TT newReference, int newStamp){


	}
};

}
