import pygame
import subprocess

SCREEN_WIDTH, SCREEN_HEIGHT = 500, 500
WORLD_WIDTH, WORLD_HEIGHT = SCREEN_WIDTH * 5, SCREEN_HEIGHT * 5
PARTS_LIMIT = 1000


class Part(pygame.sprite.Sprite):
    def __init__(self, x, y):
        super().__init__()
        self.image = pygame.Surface((1, 1))
        self.image.fill((200, 200, 200))
        self.rect = self.image.get_rect(center=(x, y))


points_handler = subprocess.Popen(
        ['./wind_backend', str(WORLD_WIDTH), str(WORLD_HEIGHT), str(PARTS_LIMIT)],
        stdout=subprocess.PIPE
)


pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
clock = pygame.time.Clock()
running = True
offset_x, offset_y = SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2
while running:
    parts = []

    line = points_handler.stdout.readline().decode().strip()
    while line != "END_LIST":
        coord = (int(line.split()[0]), int(line.split()[1]))
        parts.append(Part(*coord))
        line = points_handler.stdout.readline().decode().strip()
    parts = pygame.sprite.Group(*parts)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                running = False
                break
            elif event.key == pygame.K_RIGHT:
                offset_x += SCREEN_WIDTH
            elif event.key == pygame.K_DOWN:
                offset_y += SCREEN_HEIGHT
            elif event.key == pygame.K_LEFT:
                offset_x -= SCREEN_WIDTH
            elif event.key == pygame.K_UP:
                offset_y -= SCREEN_HEIGHT
            print((offset_x, offset_y))

    screen.fill((7, 79, 135))
    screen_rect = screen.get_rect(topleft=(offset_x, offset_y))
    for part in parts:
        if screen_rect.colliderect(part.rect):
            screen.blit(part.image, (part.rect.centerx % SCREEN_WIDTH, part.rect.centery % SCREEN_HEIGHT))
    pygame.display.flip()
pygame.quit()
