project('lwidget', 'c',
        default_options: [ 'c_std=c23', 'warning_level=everything' ],
        license: 'GPL-3.0-or-later',
        license_files: 'COPYING',
        meson_version: '>=1.8.0',
        version: 'v1.0')

add_project_dependencies([
        dependency('lua'),
        dependency('gtk+-3.0'),
        dependency('gtk-layer-shell-0'),
], language: 'c')


is_release = get_option('buildtype') == 'release'
flags = [
        '-D__release=@0@'.format(is_release),
        '-D__app_name="@0@"'.format(meson.project_name()),
        '-D__app_version="@0@"'.format(meson.project_version()),
]


source = files(
        'src/arg.c',
        'src/gtk.c',
        'src/main.c',
        'src/memory.c',
        'src/strlib.c',
        'src/types.c',
        'src/utils.c',
        'src/widgets.c',
)

executable('lwidget', source,
           include_directories: include_directories('include'),
           c_args: flags)

