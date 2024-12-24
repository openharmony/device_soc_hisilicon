#!/usr/bin/env python3
# coding=utf-8

# Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
from xml.etree import cElementTree as ElementTree

class XmlParser:
    def __init__(self):
        self.d2xml_level = 0
        self.indent = '  '

    def _dict_to_xml(self, data, root='object'):
        if isinstance(data, dict):
            xml_ret = '  ' * self.d2xml_level + f'<{root}>\n'
            self.d2xml_level += 1
            for key, value in data.items():
                xml_ret += self._dict_to_xml(value, key)
            self.d2xml_level -= 1
            xml_ret += '  ' * self.d2xml_level + f'</{root}>\n'
            return xml_ret

        if isinstance(data, (list, tuple, set)):
            xml_ret = ''
            for item in data:
                xml_ret += self._dict_to_xml(item, root)
            return xml_ret
        if data is None:
            data = ''
        xml_ret = '  ' * self.d2xml_level + f'<{root}>'
        xml_ret += str(data)
        xml_ret += f'</{root}>\n'
        return xml_ret

    def dict_to_xml(self, data, root='object'):
        self.d2xml_level = 0
        xml_str = '<?xml version="1.0" encoding="UTF-8"?>\n'
        xml_str += self._dict_to_xml(data, root)
        return xml_str

    def et2dict(self, root):
        xml_dict = {}
        if root.items():
            xml_dict.update(dict(root.items()))
        for element in root:
            if element:
                key = element.tag
                val = self.et2dict(element)
            else:
                key = element.tag
                val = element.text

            if key not in xml_dict:
                xml_dict[key] = val
                continue

            if not isinstance(xml_dict[key], list):
                xml_dict[key] = [xml_dict[key]]
            temp = xml_dict[key]
            temp.append(val)
            xml_dict[key] = temp
        return xml_dict

    def xml2dict(self, xml_file):
        tree = ElementTree.parse(xml_file)
        root = tree.getroot()
        return self.et2dict(root)

if __name__ == "__main__":
    script_dir = os.path.split(os.path.realpath(__file__))[0]
    file = os.path.join(script_dir, 'sdk_template', 'template.ewp')
    test = XmlParser()
    dict_ = test.xml2dict(file)
    xml = test.dict_to_xml(dict_, root='project')
    print(xml)
