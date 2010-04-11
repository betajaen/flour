gem 'hpricot', '>= 0.6'
require 'hpricot'


def read(type, file)
    
    vertices = []
    indexes = []
    texturecoords = []
    normals = []
    
    mesh = Hpricot::XML(File.read(file))
    id = 0
    (mesh/"submesh").each do |submesh|
    
        # vertices
        (submesh/"position").each do |vertex|
            vertices.push vertex["x"]
            vertices.push vertex["y"]
            vertices.push vertex["z"]
        end
        
        # normals
        (submesh/"normal").each do |vertex|
            normals.push vertex["x"]
            normals.push vertex["y"]
            normals.push vertex["z"]
        end
        
        # texture coords
        (submesh/"texcoord").each do |vertex|
            texturecoords.push vertex["u"]
            texturecoords.push vertex["v"]
        end
        
        # triangles
        (submesh/"face").each do |face|
            indexes.push face["v1"]
            indexes.push face["v2"]
            indexes.push face["v3"]
        end
        
        out = String.new
        out << file
        suffix = String.new
        suffix << "." + id.to_s if id != 0
        suffix << ".flower"
        
        fout = out.gsub /\.mesh\.xml/, suffix
        fout << ".flower" if (fout == out)
        
        write(fout, file, type, vertices, indexes, normals, texturecoords)
        
        id +=1
    end 
end


def write(file, original, type, vertices, indexes, normals, texturecoords) 
    
    out = String.new
    out << "# '#{original}' converted by OgreXMLToFlower\n"
    
    out << "type #{type}\n\n"
    
    # vertices
    out << "\nvertices " << vertices.join(', ') << "\n" if vertices.size > 2
    
    # indexes
    out << "\nindexes " << indexes.join(', ') << "\n" if indexes.size > 2
    
    # normals
    out << "\nnormals " << normals.join(', ') << "\n" if normals.size > 2
    
    # texturecoords
    out << "\ntexturecoords " << texturecoords.join(', ') << "\n" if texturecoords.size > 2
    
    
    File.open(file, 'w')  { |f| f.write out }
    puts "Wrote #{file}"
end

if ARGV.size == 0
    puts "OgreXML to Flower\n(c) Robin Southern, http://github.com/betajaen/nxogre"
    puts "Usage:\n  ogrexmlconverter.rb <type> <file(s)>"
    puts " - Where type is \"convex\", \"triangle\" or \"cloth\"."
    puts " - Where file(s) is a space seperated path for each mesh(s) to convert from."
    puts "Notes:\n - Enclose in \"quotations\" for any file with spaces in its path."
    puts " - SoftBody exporting is not supported."
    puts " - Submeshes are exported as seperate files."
    puts "Example:"
    puts " - ogrexmlconverter.rb triangle ogrehead.mesh.xml"
    Process.exit
end

args = ARGV.clone

type = String.new
type << ARGV[0]

type.downcase!

if ["convex", "triangle", "cloth"].include?(type) == false
    puts "I can only convert into a convex, triangle or cloth. Not a '#{type}'."
    Process.exit
end

args.delete_at 0

args.each do |arg|
    read type, arg
end
