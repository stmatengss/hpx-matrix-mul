// #include <hpx/hpx_main.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/config.hpp>
#include <hpx/include/parallel_for_each.hpp>
#include <hpx/include/parallel_move.hpp>
#include <hpx/include/partitioned_vector_predef.hpp>
#include <vector>
#include <random>

typedef std::vector<std::pair<int, std::vector<std::uint64_t> > > matrix;
typedef std::pair<int, std::vector<std::uint64_t> > row;
typedef std::vector<std::uint64_t> row_data;


void gen_matrix(matrix &m) {
    hpx::parallel::for_each(hpx::parallel::execution::par, m.begin(), m.end(), [&](row &r){
        r.second = row_data(m.size());
        for (int i = 0; i < r.second.size(); i ++ ) {
            r.second[i] = rand();
        }
    });
}

std::uint64_t mul_row(row_data r1, row_data r2) {

    std::uint64_t res;
    for (int i = 0; i < r1.size(); i ++ ) {
        res += r1[i] * r2[i];
    }

    return res;
} 

void mul(matrix m1, matrix m2, matrix &res) {
    hpx::parallel::for_each(hpx::parallel::execution::par, m1.begin(), m1.end(), [&](row r1){
        hpx::parallel::for_each(hpx::parallel::execution::par, m2.begin(), m2.end(), [&](row r2){
            res[r1.first].second[r2.first] = mul_row(r1.second, r2.second);
        });
    });
}

int hpx_main(hpx::program_options::variables_map &vm) {

    int len = vm["len"].as<int>();

    hpx::cout << "Len: " << len << hpx::flush;

    std::uint64_t t = hpx::util::high_resolution_clock::now();

    // typedef hpx::partitioned_vector<std::vector<std::uint64_t> > matrix;

    // std::vector<hpx::id_type> localities = hpx::find_all_localities();

    // matrix m1(len, hpx::container_layout(4));
    // matrix m2(len, hpx::container_layout(4));
    matrix m1(len), m2(len);
    matrix res(len);

    gen_matrix(m1);
    gen_matrix(m2);

    mul(m1, m2, res);

    std::uint64_t elapsed = hpx::util::high_resolution_clock::now() - t;

    hpx::cout << "Time: " << elapsed << hpx::flush;

    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    hpx::program_options::variables_map vm;
    hpx::program_options::options_description od;
    od.add_options()("len", hpx::program_options::value<int>());

    hpx::program_options::store(hpx::program_options::command_line_parser(argc, argv).options(od).run(), vm);

    hpx::init(od, 0, nullptr);

    return EXIT_SUCCESS;
}