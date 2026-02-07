import sys.io.File;

using Lambda;
using StringTools;

class HaxeWrapperBuilder {
	public static function main():Void {
		final root = Xml.parse(File.getContent('_build/haxe/coldsteel.xml')).firstElement();
		File.saveContent('_build/haxe/Cs.hx', getHaxeContent(root));
	}

	private static function getHaxeContent(root:Xml):String {
		final constants = getConstants(root);
		final functions = getFunctions(root);
		final classes = getClasses(functions);
		return getHaxeClasses(classes) + '\n' + getHaxeCsClass(constants, functions);
	}

	private static function getHaxeClasses(classes:Iterable<String>):String {
		return classes.map(c -> 'extern class $c${getHaxeExtends(c)} {}\n').join('');
	}

	private static function getHaxeExtends(className:String):String {
		return (className.endsWith('SceneNode') && className != 'ISceneNode') ? ' extends ISceneNode' : '';
	}

	private static function getHaxeCsClass(constants:Iterable<Constant>, functions:Iterable<Function>):String {
		return '@:native("_G") extern class Cs {\n${getHaxeConstants(constants)}\n${getHaxeMethods(functions)}}\n';
	}

	private static function getHaxeConstants(constants:Iterable<Constant>):String {
		return constants.map(c -> '\tpublic static inline final ${c.name} = ${c.value};\n').join('');
	}

	private static function getHaxeMethods(functions:Iterable<Function>):String {
		return functions.map(f -> '\t@:native("${f.name}") public static function ${getHaxeMethod(f)};\n').join('');
	}

	private static function getHaxeMethod(func:Function):String {
		final funcName = Std.string(func);
		if (funcName.startsWith('RGBA'))
			return funcName.substr(0, 4).toLowerCase() + funcName.substr(4);
		if (funcName.startsWith('RGB'))
			return funcName.substr(0, 3).toLowerCase() + funcName.substr(3);
		if (funcName.startsWith('GUI') || funcName.startsWith('XML'))
			return funcName.substr(0, 3).toLowerCase() + funcName.substr(3);
		return funcName.substr(0, 1).toLowerCase() + funcName.substr(1);
	}

	private static function getClasses(functions:Iterable<Function>):Iterable<String> {
		final primitives = ['Void', 'Bool', 'Int', 'Float', 'String', 'Any'];
		final fullList = functions.flatMap(f -> [f.type].concat(f.params.map(p -> p.type)));
		final classes = [];
		fullList.iter(c -> if (!classes.contains(c) && !primitives.contains(c)) classes.push(c));
		return classes;
	}

	private static function getConstants(element:Xml):Iterable<Constant> {
		final constants = XmlParser.elements(element, 'constant').map(c -> Constant.fromXml(c));
		final childConstants = XmlParser.elements(element).flatMap(c -> getConstants(c));
		return constants.concat(childConstants);
	}

	private static function getFunctions(element:Xml):Iterable<Function> {
		final functions = XmlParser.elements(element, 'cdecl').filter(c -> XmlParser.attribute(c, 'kind') == 'function').map(f -> Function.fromXml(f));
		final childFunctions = XmlParser.elements(element).flatMap(c -> getFunctions(c));
		return functions.concat(childFunctions).filter(f -> !f.name.startsWith('_'));
	}
}

private class Constant {
	public final name:String;
	public final value:String;

	public function new(name:String, value:String) {
		this.name = name;
		this.value = value;
	}

	public static function fromXml(element:Xml):Constant {
		return new Constant(XmlParser.attribute(element, 'name'), XmlParser.attribute(element, 'value'));
	}

	public function toString():String {
		return '$name=$value';
	}
}

private class Function {
	public final name:String;
	public final type:String;
	public final params:Iterable<Param>;

	public function new(name:String, type:String, params:Iterable<Param>) {
		this.name = name;
		this.type = type;
		this.params = params.array();
	}

	public static function fromXml(element:Xml):Function {
		final attributelistElem = XmlParser.elements(element, 'attributelist')[0];
		final parmlistElem = XmlParser.elements(attributelistElem, 'parmlist')[0];
		final params = (parmlistElem != null) ? XmlParser.elements(parmlistElem, 'parm') : [];
		return new Function(XmlParser.attribute(element, 'name'), XmlParser.type(XmlParser.attribute(element, 'type')), params.map(p -> Param.fromXml(p)));
	}

	public function toString():String {
		return '$name(${params.map(p -> p.toString()).join(', ')}):$type';
	}
}

private class Param {
	public final name:String;
	public final type:String;

	public function new(name:String, type:String) {
		this.name = name;
		this.type = type;
	}

	public static function fromXml(element:Xml):Param {
		return new Param(XmlParser.attribute(element, 'name'), XmlParser.type(XmlParser.attribute(element, 'type')));
	}

	public function toString():String {
		return '$name:$type';
	}
}

private class XmlParser {
	public static function attribute(element:Xml, name:String):String {
		final attributes = element.elementsNamed('attributelist').next().elementsNamed('attribute');
		for (attr in attributes) {
			if (attr.get('name') == name) {
				return attr.get('value');
			}
		}
		return '';
	}

	public static function elements(root:Xml, ?name:String):Array<Xml> {
		final it = (name != null) ? root.elementsNamed(name) : root.elements();
		return [for (el in it) el];
	}

	public static function type(type:String):String {
		if (type == 'void') {
			return 'Void';
		} else if (type == 'bool' || type == 'bool_t') {
			return 'Bool';
		} else if (type == 'int') {
			return 'Int';
		} else if (type == 'float') {
			return 'Float';
		} else if (type == 'p.q(const).char' || type == 'q(const).char') {
			return 'String';
		} else if (type == "p.void") {
			return 'Any';
		} else if (type.startsWith('p.q(const).')) {
			return type.substr(11);
		} else if (type.startsWith('p.')) {
			return type.substr(2);
		} else {
			return type;
		}
	}
}
