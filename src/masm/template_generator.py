import argparse
import os.path
import re



def parse_masm(path):
    lst = []
    for line in open(path).readlines():
        if line.startswith("#define") and line.find("MASM_"):
            toks = line.split()
            lst.append(toks[1])
    return lst


def generate_pretty_print(lst, out_file):
    fd = open(out_file, "w")
    for d in lst:
        fd.write(f"case {d} : return \"{d}\";\n")
    fd.close()


def read_unittest_data(in_file):
    for_test_list = []
    for_lexer_list = []
    for_kw_list = []
    regex = '^[A-Za-z][A-Za-z0-9]*'
    for ln in open(in_file, "r").readlines():
        line = ln.strip()
        if len(line) == 0:
            continue
        if line.startswith("#"):
            continue
        toks = line.split(":")
        name = toks[0]
        value = toks[1].strip()
        for_test_only = False
        if name.startswith('-'):
            name = name[1:]
            for_test_only = True
        for_test_list.append([name, value])
        if not for_test_only:
            if re.search(regex, value):
                for_kw_list.append([name, value])
            else:
                for_lexer_list.append([name, value])
    return for_test_list, for_lexer_list, for_kw_list


def build_test(lst, out_file):
    fd = open(out_file, "w")
    for d in lst:
        name, value = d
        fd.write(f"REQUIRE({name} == masm::test::scan_one_token (\"{value}\"));\n")
    fd.close()


def build_lexer(lst, out_file):
    fd = open(out_file, "w")
    for d in lst:
        name, value = d
        fd.write(f"\"{value}\" {{return {name};}}\n")
    fd.close()


def build_kw(lst, out_file):
    fd = open(out_file, "w")
    for d in lst:
        name, value = d
        fd.write(f"if (dTEST_MACRO(\"{value}\")) {{return {name};}}\n")
    fd.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Template generator",
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("-m", "--masm-input", help="MASM input .h file", required=True)
    parser.add_argument("-p", "--generate-print", help="Generate pretty print stub")
    parser.add_argument("-t", "--generate-test", help="Generate lexer and test")
    args = parser.parse_args()
    config = vars(args)

    masm_input = config["masm_input"]
    base_dir = os.path.dirname(masm_input)

    masm_lst = parse_masm(masm_input)

    if config["generate_print"]:
        ofile = os.path.join(base_dir, config["generate_print"])
        generate_pretty_print(masm_lst, ofile)
    if config["generate_test"]:
        ofile = os.path.basename(config["generate_test"])
        filename, file_extension = os.path.splitext(ofile)
        tfile = os.path.join(base_dir, "test_" + filename + ".h")
        lfile = os.path.join(base_dir, "lexer_" + filename + ".inc")
        kwfile = os.path.join(base_dir, "lexer_kw_" + filename + ".h")

        for_test, for_lexer, for_kw = read_unittest_data(config["generate_test"])
        build_test(for_test, tfile)
        build_lexer(for_lexer, lfile)
        build_kw(for_kw, kwfile)
