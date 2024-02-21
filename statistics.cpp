#include <iostream>
#include <limits>

class IStatistics {
public:
	virtual ~IStatistics() {}

	virtual void update(double next) = 0;
	virtual double eval() const = 0;
	virtual const char * name() const = 0;
};

class Min : public IStatistics {
public:
	Min() : m_min{std::numeric_limits<double>::max()} {}

	void update(double next) override {
		if (next < m_min) {
			m_min = next;
		}
	}

	double eval() const override { return m_min;}
	const char * name() const override { return "min";}

private:
	double m_min;
};



class Max : public IStatistics {
public:
	Max() : m_max{std::numeric_limits<double>::min()}, is_first(true) {}
	
	void update(double next) override {
		if (is_first){
			is_first=false;
			m_max=next;
			return;
		}
		if (next > m_max) {
			m_max = next;
		}
	}

	double eval() const override { return m_max;}
	const char * name() const override { return "max";}

private:
	double m_max;
	bool is_first;
};

class Mean : public IStatistics {
public:
	Mean() : m_mean(0), count(0) {}

	void update(double next) override {
		if (count==0){
			count = 1;
			m_mean = next;
			return;
		}
		m_mean = ((m_mean*count)+next)/(count+1);
		count++;
	}

	double eval() const override { return m_mean;}
	const char * name() const override { return "mean";}

private:
	double m_mean;
	unsigned int count;
};

int main() {

	const size_t statistics_count = 3;
	IStatistics *statistics[statistics_count];

	statistics[0] = new Min{};
	statistics[1] = new Max{};
	statistics[2] = new Mean{};

	double val = 0;
	while (std::cin >> val) {
		for (size_t i = 0; i < statistics_count; ++i) {
			statistics[i]->update(val);
		}
	}

	// Handle invalid input data
	if (!std::cin.eof() && !std::cin.good()) {
		std::cerr << "Invalid input data\n";
		return 1;
	}

	// Print results if any
	for (size_t i = 0; i < statistics_count; ++i) {
		std::cout << statistics[i]->name() << " = " << statistics[i]->eval() << std::endl;
	}

	// Clear memory - delete all objects created by new
	for (size_t i = 0; i < statistics_count; ++i) {
		delete statistics[i];
	}

	return 0;
}