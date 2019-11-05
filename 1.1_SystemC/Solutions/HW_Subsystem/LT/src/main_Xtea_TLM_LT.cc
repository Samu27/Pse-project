#include "Xtea_TLM_LT.hh"
#include "Xtea_TLM_LT_testbench.hh"

class Xtea_TLM_LT_top : public sc_module {
  public:
    Xtea_TLM_LT_top(sc_module_name name) : sc_module(name)
        , m_target("target"), m_initiator("initiator") {
      // Initiator - target
      m_initiator.initiator_socket(m_target.target_socket);
    }

  private:
    Xtea_TLM_LT  m_target;
    Xtea_TLM_LT_testbench m_initiator;

};

int main(int argc, char* argv[]) {
  Xtea_TLM_LT_top top("top");
  sc_start();

  return 0;
}
