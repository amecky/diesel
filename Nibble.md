class PipelineState {
    
struct Entry {
    uint16_t type;
    uint8_t slot;
    uint8_t data;
};
    
public:
    void reset() {
         _index = 0;
    }
    bool isUsed(RID rid) {
        return isUsed(rid,0,0);                
    }
    bool isUsed(RID rid,uint8_t slot,uint8_t data) {
        int type = type_mask(rid);
        for ( int i = 0; i < _index; ++i ) {
            const Entry& e = _entries[i];
            if ( e.type == type && e.slot == slot && e.data == data) {
                return true;
            }
        }
        return false;
    }
    void add(RID rid) {
        add(rid,0,0);
    }
    void add(RID rid,int slot,int type);
private:
    void alloc(uint16_t newCapacity);
    Entry* _entries;
    uint16_t _capacity;
    uint16_t _index;
}; 