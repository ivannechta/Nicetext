#ifndef POSTDATA_H__
#define POSTDATA_H__

// Class for keeping track of posting data

class MTCpostData {
private:
	MTCstring param;
	MTCstring value;

public:
	MTCpostData() {};
	MTCpostData(const MTCpostData &pd) { param=pd.param; value=pd.value;}
	virtual ~MTCpostData() {};

	void setParam(MTCstring &aStr) { param=aStr; };
	void setParam(const char *aStr) { param=aStr; };
	const MTCstring & getParam() const { return param; };
	void setValue(MTCstring &aStr) { value=aStr; };
	void setValue(const char *aStr) { value=aStr; };
	const MTCstring & getValue() const { return value; };

	MTCpostData & operator=(const MTCpostData &pd)
	{
		param=pd.param; value=pd.value;
	}

	int operator==(const MTCpostData &pd)
	{
		return (param==pd.param);
	}

	int operator<(const MTCpostData &pd)
	{
		return (param<pd.param);
	}
};

#endif POSTDATA_H__
