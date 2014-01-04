/*
 * syn_fifo.h
 *
 *  Created on: 2014/01/04
 *      Author: Kuniyasu
 */

#ifndef SYN_FIFO_H_
#define SYN_FIFO_H_

#include <systemc.h>

#define TR_NAME(_name) (std::string(name())+"_"+std::string(_name)).c_str()
#define PIN_NAME(bname,_name_) (std::string(bname)+"_"+std::string(_name_)).c_str()

template<class DT>class fifo_get_if;
template<class DT>class fifo_put_if;

template<class DT,unsigned int SIZE, unsigned int WIDTH>class syn_fifo_base_put;
template<class DT,unsigned int SIZE, unsigned int WIDTH>class syn_fifo_base_get;
template<class DT,unsigned int SIZE, unsigned int WIDTH>class syn_fifo_put;
template<class DT,unsigned int SIZE, unsigned int WIDTH>class syn_fifo_get;
template<class DT,unsigned int SIZE, unsigned int WIDTH>class syn_channel;
template<class DT,unsigned int SIZE, unsigned int WIDTH>class syn_channel_in;

template<class DT>class fifo_get_if:public sc_interface{
public:
	virtual void r_reset() = 0;
	virtual bool is_not_empty() = 0;
	virtual bool nb_get(DT& dt) = 0;
	virtual void  b_get(DT& dt) = 0;
};

template<class DT>class fifo_put_if:public sc_interface{
public:
	virtual void w_reset() = 0;
	virtual bool is_not_full() = 0;
	virtual bool nb_put(const DT& dt) = 0;
	virtual void  b_put(const DT& dt) = 0;
};


template<class DT,unsigned int SIZE, unsigned int WIDTH>class syn_fifo_base_put{
public:
	typedef sc_uint<WIDTH> cnt_type;

	sc_out<bool> w_flag;
	sc_in<bool> r_flag;

	sc_out<cnt_type> w_cnt;
	sc_in<cnt_type> r_cnt;

	sc_out<DT> data[SIZE];

	syn_fifo_base_put(const char* name=sc_gen_unique_name("syn_fifo_base_put")){}

	template<class P> void bind(P& p){
		w_flag(p.w_flag);
		r_flag(p.r_flag);
		w_cnt(p.w_cnt);
		r_cnt(p.r_cnt);

		for(unsigned int c=0; c<SIZE; c++){
			data[c](p.data[c]);
		}
	}

};

template<class DT,unsigned int SIZE, unsigned int WIDTH>class syn_fifo_base_get{
public:
	typedef sc_uint<WIDTH> cnt_type;

	sc_in<bool>  w_flag;
	sc_out<bool> r_flag;

	sc_in<cnt_type>  w_cnt;
	sc_out<cnt_type> r_cnt;

	sc_in<DT> data[SIZE];

	syn_fifo_base_get(const char* name=sc_gen_unique_name("syn_fifo_base_get")){}

	template<class P> void bind(P& p){
		w_flag(p.w_flag);
		r_flag(p.r_flag);
		w_cnt(p.w_cnt);
		r_cnt(p.r_cnt);

		for(unsigned int c=0; c<SIZE; c++){
			data[c](p.data[c]);
		}
	}

};


template<class DT,unsigned int SIZE, unsigned int WIDTH>class syn_fifo_put:public syn_fifo_base_put<DT,SIZE,WIDTH>, public fifo_put_if<DT>{
public:
	typedef sc_uint<WIDTH> cnt_type;
	typedef syn_fifo_base_put<DT,SIZE,WIDTH> base_type;

	syn_fifo_put(const char* name=sc_gen_unique_name("syn_fifo_put")):base_type(name),w_flag_in(false),w_cnt_in(0){}

	template<class P> void bind(P& p){
		base_type::w_flag(p.w_flag);
		base_type::r_flag(p.r_flag);
		base_type::w_cnt(p.w_cnt);
		base_type::r_cnt(p.r_cnt);

		for(unsigned int c=0; c<SIZE; c++){
			base_type::data[c](p.data[c]);
		}
	}

	virtual void w_reset(){}

	virtual bool is_not_full(){
		return false;
	}

	virtual bool nb_put(const DT& dt){
		return false;
	}

	virtual void  b_put(const DT& dt){

	}

private:
	bool 	 w_flag_in;
	cnt_type w_cnt_in;
};

template<class DT,unsigned int SIZE, unsigned int WIDTH>class syn_fifo_get:public syn_fifo_base_get<DT,SIZE,WIDTH>, public fifo_get_if<DT>{
public:
	typedef sc_uint<WIDTH> cnt_type;
	typedef syn_fifo_base_get<DT,SIZE,WIDTH> base_type;

	syn_fifo_get(const char* name=sc_gen_unique_name("syn_fifo_get")):base_type(name),r_flag_in(false),r_cnt_in(0){}
	~syn_fifo_get(){}

	template<class P> void bind(P& p){
		base_type::w_flag(p.w_flag);
		base_type::r_flag(p.r_flag);
		base_type::w_cnt(p.w_cnt);
		base_type::r_cnt(p.r_cnt);

		for(unsigned int c=0; c<SIZE; c++){
			base_type::data[c](p.data[c]);
		}
	}

	virtual void r_reset(){}
	virtual bool is_not_empty(){
		return false;
	}

	virtual bool nb_get(DT& dt){
		return false;
	}

	virtual void  b_get(DT& dt){}

private:
	bool 	 r_flag_in;
	cnt_type r_cnt_in;
};


template<class DT,unsigned int SIZE, unsigned int WIDTH>class syn_channel{
public:
	typedef sc_uint<WIDTH> cnt_type;

	sc_signal<bool> w_flag;
	sc_signal<bool> r_flag;

	sc_signal<cnt_type> w_cnt;
	sc_signal<cnt_type> r_cnt;

	sc_signal<DT> data[SIZE];

	syn_channel(const char* name=sc_gen_unique_name("syn_channel")){}
};

template<class DT,unsigned int SIZE, unsigned int WIDTH>class syn_channel_in:public syn_channel<DT,SIZE,WIDTH>,
public fifo_get_if<DT>, public fifo_put_if<DT>{

public:
	typedef sc_uint<WIDTH> cnt_type;
	typedef syn_channel<DT,SIZE,WIDTH> base_type;

	syn_channel_in(const char* name=sc_gen_unique_name("syn_channel_in")):base_type(name),r_flag_in(false),r_cnt_in(0),w_flag_in(false),w_cnt_in(0){}

private:
	bool 	 r_flag_in;
	cnt_type r_cnt_in;
	bool 	 w_flag_in;
	cnt_type w_cnt_in;

};

template<class DT> class syn_fifo_2{
public:
	static const unsigned int SIZE  = 2U;
	static const unsigned int WIDTH = 1U;
	typedef sc_uint<WIDTH> cnt_type;

	typedef syn_fifo_base_get<DT,SIZE,WIDTH> base_get;
	typedef syn_fifo_base_put<DT,SIZE,WIDTH> base_put;
	typedef syn_fifo_get<DT,SIZE,WIDTH> get;
	typedef syn_fifo_put<DT,SIZE,WIDTH> put;
	typedef syn_channel<DT,SIZE,WIDTH>  channel;
};

template<class DT> class syn_fifo_3{
public:
	static const unsigned int SIZE  = 3U;
	static const unsigned int WIDTH = 2U;
	typedef sc_uint<WIDTH> cnt_type;

	typedef syn_fifo_base_get<DT,SIZE,WIDTH> base_get;
	typedef syn_fifo_base_put<DT,SIZE,WIDTH> base_put;
	typedef syn_fifo_get<DT,SIZE,WIDTH> get;
	typedef syn_fifo_put<DT,SIZE,WIDTH> put;
	typedef syn_channel<DT,SIZE,WIDTH>  channel;
};

template<class DT> class syn_fifo_4{
public:
	static const unsigned int SIZE  = 4U;
	static const unsigned int WIDTH = 2U;
	typedef sc_uint<WIDTH> cnt_type;

	typedef syn_fifo_base_get<DT,SIZE,WIDTH> base_get;
	typedef syn_fifo_base_put<DT,SIZE,WIDTH> base_put;
	typedef syn_fifo_get<DT,SIZE,WIDTH> get;
	typedef syn_fifo_put<DT,SIZE,WIDTH> put;
	typedef syn_channel<DT,SIZE,WIDTH>  channel;
};

template<class DT> class syn_fifo_5{
public:
	static const unsigned int SIZE  = 5U;
	static const unsigned int WIDTH = 3U;
	typedef sc_uint<WIDTH> cnt_type;

	typedef syn_fifo_base_get<DT,SIZE,WIDTH> base_get;
	typedef syn_fifo_base_put<DT,SIZE,WIDTH> base_put;
	typedef syn_fifo_get<DT,SIZE,WIDTH> get;
	typedef syn_fifo_put<DT,SIZE,WIDTH> put;
	typedef syn_channel<DT,SIZE,WIDTH>  channel;
};

template<class DT> class syn_fifo_6{
public:
	static const unsigned int SIZE  = 6U;
	static const unsigned int WIDTH = 3U;
	typedef sc_uint<WIDTH> cnt_type;

	typedef syn_fifo_base_get<DT,SIZE,WIDTH> base_get;
	typedef syn_fifo_base_put<DT,SIZE,WIDTH> base_put;
	typedef syn_fifo_get<DT,SIZE,WIDTH> get;
	typedef syn_fifo_put<DT,SIZE,WIDTH> put;
	typedef syn_channel<DT,SIZE,WIDTH>  channel;
};

template<class DT> class syn_fifo_7{
public:
	static const unsigned int SIZE  = 7U;
	static const unsigned int WIDTH = 3U;
	typedef sc_uint<WIDTH> cnt_type;

	typedef syn_fifo_base_get<DT,SIZE,WIDTH> base_get;
	typedef syn_fifo_base_put<DT,SIZE,WIDTH> base_put;
	typedef syn_fifo_get<DT,SIZE,WIDTH> get;
	typedef syn_fifo_put<DT,SIZE,WIDTH> put;
	typedef syn_channel<DT,SIZE,WIDTH>  channel;
};

template<class DT> class syn_fifo_8{
public:
	static const unsigned int SIZE  = 8U;
	static const unsigned int WIDTH = 3U;
	typedef sc_uint<WIDTH> cnt_type;

	typedef syn_fifo_base_get<DT,SIZE,WIDTH> base_get;
	typedef syn_fifo_base_put<DT,SIZE,WIDTH> base_put;
	typedef syn_fifo_get<DT,SIZE,WIDTH> get;
	typedef syn_fifo_put<DT,SIZE,WIDTH> put;
	typedef syn_channel<DT,SIZE,WIDTH>  channel;
};
#endif /* SYN_FIFO_H_ */
