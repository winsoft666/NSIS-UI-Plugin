import os
import sys

g_extract_file_cmd_total = 0
g_create_dir_cmd_total = 0
g_cur_script_index = 0
g_insert_nsis_script_list = []
g_root_dir = ""



def nsis_cmd_num_statistics(dir):
    global g_extract_file_cmd_total
    global g_create_dir_cmd_total
    if not os.path.isdir(dir):
        return

    files = os.listdir(dir)
    for fi in files:
        fi_d = os.path.join(dir, fi)
        if os.path.isdir(fi_d):
            g_create_dir_cmd_total += 1
            nsis_cmd_num_statistics(fi_d)
        else:
            g_extract_file_cmd_total += 1


def generate_nsis_script(dir):
    global g_cur_script_index
    if not os.path.isdir(dir):
        return

    for fi in os.listdir(dir):
        full_path = os.path.join(dir, fi)
        if os.path.isdir(full_path):
            sub_dir = full_path[len(g_root_dir):]
            g_cur_script_index += 1
            g_insert_nsis_script_list.append('    CreateDirectory "$INSTDIR{0}"'.format(sub_dir))
            g_insert_nsis_script_list.append('    {0}::SetInstallStepDescription "Create Directory: {1}" {2}'
                                             .format('${UI_PLUGIN_NAME}', '$INSTDIR' + sub_dir,
                                                     g_cur_script_index * 100 / (g_extract_file_cmd_total + g_create_dir_cmd_total)))
            generate_nsis_script(full_path)
        else:
            g_cur_script_index += 1
            g_insert_nsis_script_list.append('    SetOutPath "$INSTDIR{0}"'.format(dir[len(g_root_dir):]))
            g_insert_nsis_script_list.append('    File "{0}"'.format( full_path))
            g_insert_nsis_script_list.append('    {0}::SetInstallStepDescription "Extract File: $INSTDIR{1}" {2}'
                                             .format('${UI_PLUGIN_NAME}', full_path[len(g_root_dir):], g_cur_script_index * 100 / (g_extract_file_cmd_total + g_create_dir_cmd_total)))


def do_main(nsis_script_template_path):
    global g_root_dir
    if g_root_dir[len(g_root_dir) - 1] == '\\':
        g_root_dir = g_root_dir[:len(g_root_dir) - 1]
    nsis_cmd_num_statistics(g_root_dir)
    print "file total: " + str(g_extract_file_cmd_total) + ", dir total: " + str(g_create_dir_cmd_total)

    generate_nsis_script(g_root_dir)
    f = open(nsis_script_template_path, "r")
    all_nsis_script_lines = []
    cur_line_index = -1
    insert_line_index = -1
    for s in f.readlines():
        all_nsis_script_lines.append(s)
        cur_line_index += 1
        if s.find('Function ___ExtractFiles') != -1:
            insert_line_index = cur_line_index + 1
    f.close()

    if insert_line_index == -1:
        return

    for s in g_insert_nsis_script_list:
        all_nsis_script_lines.insert(insert_line_index, s)
        insert_line_index += 1

    nsis_script_path = nsis_script_template_path[:len(nsis_script_template_path) - len('-template.nsi')]
    nsis_script_path += '.nsi'
    f = open(nsis_script_path, "w")
    all_script = '\n'.join(all_nsis_script_lines)
    f.write(all_script)
    f.close()

if __name__ == '__main__':
    nsis_script_template_path = sys.argv[1]
    print nsis_script_template_path
    g_root_dir = sys.argv[2]
    print  g_root_dir
    do_main(nsis_script_template_path)