import yaml

class Settings:
	def __init__(self, settDict):
		self.name = settDict['name']
		self.type = settDict['type']
		self.default = settDict['default']



class Module:
	def __init__(self, modDict):
		self.name = modDict['name']
		self.category = modDict['category']
		self.inputs = modDict['inputs']
		self.outputs = modDict['outputs']
		self.settings = []
		settings = modDict['settings']
		for settDict in settings:
			self.settings.append (Settings(settDict))	



	def generateModelHeader(self, stream):
		stream.write('struct {0}_Model\n\t: public Node\n\t, public std::enable_shared_from_this<{0}_Model>\n{{\n'.format(self.name))

		stream.write('\tclass Listener : public Node::Listener\n\t{\n\tpublic:\n')
		for setting in self.settings:
			stream.write('\t\tvirtual void {0}Changed({1} {0}) = 0;\n'.format(setting.name, setting.type))

		stream.write('\tprivate:\n\t\tvoid valueTreePropertyChanged (ValueTree&, const Identifier&) override;\n\t};\n\n')

		stream.write('\t{0}_Model(const ValueTree&);\n\n'.format(self.name))

		for setting in self.settings:
			stream.write('\tvoid set{0}({1} value);\n'.format(setting.name.capitalize(), setting.type))
			stream.write('\t{1} get{0}() const;\n\n'.format(setting.name.capitalize(), setting.type))

		stream.write('\tstd::function<std::unique_ptr<AudioNode> ()> getAudioFactory() override;\n')
		stream.write('\tstd::function<std::unique_ptr<JuceAudioNode> ()> getUIFactory() override;\n\n')

		stream.write('};\n\n')



	def generateModelSource(self, stream):
		stream.write('void {0}_Model::Listener::valueTreePropertyChanged(ValueTree &tree, const Identifier &id)\n{{\n'.format(self.name))
		for setting in self.settings:
			stream.write('\tif(id.toString() == {1})\n\t{{\n\t\t{0}Changed(tree.getProperty(id));\n\t}}\n'.format(setting.name, setting.name.capitalize()))

		stream.write('}\n\n')

		stream.write('{0}_Model::{0}_Model(const ValueTree &tree)\n\t: Node(tree)\n{{ }}\n\n'.format(self.name))

		for setting in self.settings:
			stream.write('void {0}_Model::set{1}({2} val)\n{{\n'.format(self.name, setting.name.capitalize(), setting.type))
			stream.write('\tm_tree.setProperty({0}, val, nullptr);\n}}\n\n'.format(setting.name.capitalize(), setting.name))

			stream.write('{2} {0}_Model::get{1}() const\n{{\n'.format(self.name, setting.name.capitalize(), setting.type))
			stream.write('\treturn m_tree.getProperty({0}, Default{0});\n}}\n\n'.format(setting.name.capitalize()))


		stream.write('std::function<std::unique_ptr<AudioNode> ()> {0}_Model::getAudioFactory()\n{{\n'.format(self.name))
		stream.write('\treturn [&](){{return std::make_unique<{0}>(this->shared_from_this());}};\n}}\n\n'.format(self.name))


		stream.write('std::function<std::unique_ptr<JuceAudioNode> ()> {0}_Model::getUIFactory()\n{{\n'.format(self.name))
		stream.write('\treturn [&](){{return std::make_unique<{0}_GUI>(this->shared_from_this());}};\n}}\n\n'.format(self.name))




	def generateAudioModuleHeader(self, stream):
		stream.write('class {0}\n\t: public AudioNode\n\t, public {0}_Model::Listener\n{{\npublic:\n'.format(self.name))
		stream.write('\t{0}(const std::shared_ptr<{0}_Model>&);\n\n'.format(self.name))
		stream.write('\tvoid process() override;\nprivate:\n')
		
		stream.write('\tstd::shared_ptr<{0}_Model> m_model;\n'.format(self.name))
		for setting in self.settings:
			stream.write('\tstd::atomic<{1}> m_{0};\n'.format(setting.name, setting.type))
		stream.write('\tstd::atomic_bool m_settingsChanged;\n\n')

		for setting in self.settings:
			stream.write('\tvoid {0}Changed({1} {0}) override;\n'.format(setting.name, setting.type))

		stream.write('\n')
		stream.write('\tvoid updateSettings();\n\n')

		for count, input in enumerate(self.inputs):
			stream.write('\tconst unsigned int {0}BufferIndex = {1};\n'.format(input, count))

		for count, output in enumerate(self.outputs):
			stream.write('\tconst unsigned int {0}BufferIndex = {1};\n'.format(output, count))


		stream.write('\n\t//Place your own variables\n\n')

		stream.write('};\n\n')




	def generateAudioModuleBaseSource(self, stream):
		stream.write('{0}::{0}(const std::shared_ptr<{0}_Model>& model)\n'.format(self.name))
		stream.write('\t: AudioNode ({0}, {1})\n'.format(len(self.inputs), len(self.outputs)))
		stream.write('\t, m_model (model)\n')

		for setting in self.settings:
			stream.write('\t, m_{0} (m_model->get{1}())\n'.format(setting.name, setting.name.capitalize()))

		stream.write('\t, m_settingsChanged (true)\n{\n\tm_model->addListener(this);\n}\n\n')	

		for setting in self.settings:
			stream.write('void {0}::{1}Changed({2} {1})\n{{\n'.format(self.name, setting.name, setting.type))
			stream.write('\tm_{0} = {0};\n\tm_settingsChanged = true;\n}}\n\n'.format(setting.name, setting.name.capitalize()))		




	def generateAudioModuleSource(self, stream):	
		stream.write('void {0}::updateSettings()\n{{\n'.format(self.name))
		stream.write('\tif(m_settingsChanged)\n\t{\n')

		stream.write('\t\tm_settingsChanged = false;\n\t\t//Set configuration here\n\t}\n}\n\n')

		stream.write('void {0}::process()\n{{\n\tupdateSettings();\n\n'.format(self.name))

		for input in self.inputs:
			stream.write('\tauto {0}Buffer = getInputData({0});\n'.format(input))
		stream.write('\n')

		for output in self.outputs:
			stream.write('\tauto {0}Buffer = getOutputData({0});\n'.format(output))
		stream.write('\n')

		stream.write('\t//Processing goes here\n}\n\n')



	def generateUIModuleHeader(self, stream):
		stream.write('class {0}_GUI\n\t: public JuceAudioNode\n{{\npublic:\n'.format(self.name))
		stream.write('\t{0}_GUI(const std::shared_ptr<{0}_Model>&);\n\n'.format(self.name))
		stream.write('\tvoid setContent(Rectangle<int> &r) override;\nprivate:\n')
		stream.write('\tstd::shared_ptr<{0}_Model> m_model;\n'.format(self.name))
		stream.write('};\n\n')




	def generateUIModuleSource(self, stream):
		stream.write('{0}_GUI::{0}_GUI(const std::shared_ptr<{0}_Model>& model)\n'.format(self.name))
		stream.write('\t: JuceAudioNode (model, {0}, {1})\n'.format(len(self.inputs), len(self.outputs)))
		stream.write('\t, m_model (model)\n{\n\t//Here add components and listener\n}\n\n')
		stream.write('void {0}_GUI::setContent(Rectangle<int> &r)\n{{\n\t//Here set components geometry\n}}\n\n'.format(self.name))




	def	generateNamespace(self, stream):
		stream.write('namespace\n{\n\n')
		stream.write('REGISTER_FACTORY({1}, {0})\n\n}}\n\n'.format(self.name, self.category))



	def	generateBaseNamespace(self, stream):
		stream.write('namespace\n{\n\n')

		for setting in self.settings:
			stream.write('const static String {0} = "{1}";\n'.format(setting.name.capitalize(), setting.name))
			stream.write('const static {1} Default{0} = {2};\n\n'.format(setting.name.capitalize(), setting.type, setting.default))
		stream.write('}\n\n')




	def generateHeader(self, stream):
		stream.write('#pragma once\n\n')
		stream.write('#include "common/juceaudionode.h"\n\n')
		self.generateModelHeader(stream)
		stream.write("//------------------//\n\n")
		self.generateAudioModuleHeader(stream)
		stream.write("//------------------//\n\n")
		self.generateUIModuleHeader(stream)
		stream.write('\n')




	def generateBaseSource(self, stream):
		stream.write('#include "{0}.h"\n\n'.format(self.name.lower()))

		self.generateBaseNamespace(stream)
		stream.write("//------------------//\n\n")
		self.generateModelSource(stream)
		stream.write("//------------------//\n\n")
		self.generateAudioModuleBaseSource(stream)
		stream.write('\n')




	def generateSource(self, stream):
		stream.write('#include "{0}.h"\n\n#include "nodefactory.h"\n\n'.format(self.name.lower()))

		self.generateNamespace(stream)
		stream.write("//------------------//\n\n")
		self.generateAudioModuleSource(stream)
		stream.write("//------------------//\n\n")
		self.generateUIModuleSource(stream)
		stream.write('\n')





	def generateCode (self, path):
		with open(path + self.name.lower() + ".h", 'w') as stream:
			self.generateHeader(stream)

		with open(path + self.name.lower() + "_base" + ".cpp", 'w') as stream:
			self.generateBaseSource(stream)

		with open(path + self.name.lower() + ".cpp", 'w') as stream:
			self.generateSource(stream)

	

with open("Modules.yaml", 'r') as stream:
    try:
        modules = yaml.load(stream)

    except yaml.YAMLError as exc:
        print(exc)

for moduledict in modules['modules']:
	module = Module(moduledict)
	module.generateCode("./")



