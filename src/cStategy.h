class cStrategy
{
public:
    void read(const std::string &fname);

    std::string text() const;

private:
    std::string myName;
    std::vector<std::pair<std::string, float>> myResult;
    float myProfit;
    float myMaxLoss;
    float myRank;
    static std::vector<cStrategy> theStrategy;

    void parse(const std::string &line);

    void add(
        const std::string &date,
        float value);

    void rank();
};
